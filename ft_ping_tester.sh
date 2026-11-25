#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FT_PING_BIN="${FT_PING_BIN:-$ROOT_DIR/ft_ping}"
SYS_PING_BIN="${SYS_PING_BIN:-$(command -v ping || true)}"
TIMEOUT_DURATION="${TIMEOUT_DURATION:-5}"
RUN_SUDO="${RUN_SUDO:-0}"

if [[ ! -x "$FT_PING_BIN" ]]; then
    echo "[INFO] ft_ping introuvable ou non executable: $FT_PING_BIN"
    echo "[INFO] Build auto via make..."
    if ! make -C "$ROOT_DIR"; then
        echo "[FATAL] Build echoue, impossible de lancer les tests."
        exit 1
    fi
fi

if [[ -z "$SYS_PING_BIN" || ! -x "$SYS_PING_BIN" ]]; then
    echo "[FATAL] ping systeme introuvable. Installe inetutils-ping (ou ping) puis relance."
    exit 1
fi

if [[ "$RUN_SUDO" == "1" ]]; then
    FT_PING_CMD=(sudo "$FT_PING_BIN")
    SYS_PING_CMD=(sudo "$SYS_PING_BIN")
else
    FT_PING_CMD=("$FT_PING_BIN")
    SYS_PING_CMD=("$SYS_PING_BIN")
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

total=0
same_exit=0
same_signal=0
same_output=0
segfault_ft=0
segfault_sys=0

normalize_output() {
    sed -E \
        -e 's/[0-9]+\.[0-9]+ ms/<TIME>/g' \
        -e 's/time=[0-9]+\.?[0-9]* ?ms/time=<TIME>/g' \
        -e 's/icmp_seq=[0-9]+/icmp_seq=<N>/g' \
        -e 's/ttl=[0-9]+/ttl=<N>/g' \
        -e 's/bytes from [^:]+:/bytes from <HOST>:/g' \
        -e 's/PING [^ ]+ \([^)]*\)/PING <HOST> (<IP>)/g'
}

decode_status() {
    local status="$1"
    if (( status >= 128 )); then
        echo "signal:$((status - 128))"
    else
        echo "exit:$status"
    fi
}

run_one() {
    local bin_name="$1"
    shift
    local out_file="$1"
    shift

    set +e
    timeout "$TIMEOUT_DURATION" "$@" >"$out_file" 2>&1
    local st=$?
    set -e

    echo "$st"
}

run_case() {
    local title="$1"
    shift
    local args=("$@")

    local ft_out="$TMP_DIR/ft_${total}.txt"
    local sys_out="$TMP_DIR/sys_${total}.txt"
    local ft_norm="$TMP_DIR/ft_${total}.norm"
    local sys_norm="$TMP_DIR/sys_${total}.norm"

    local ft_st sys_st

    ft_st="$(run_one ft "$ft_out" "${FT_PING_CMD[@]}" "${args[@]}")"
    sys_st="$(run_one sys "$sys_out" "${SYS_PING_CMD[@]}" "${args[@]}")"

    normalize_output <"$ft_out" >"$ft_norm"
    normalize_output <"$sys_out" >"$sys_norm"

    local ft_decoded sys_decoded
    ft_decoded="$(decode_status "$ft_st")"
    sys_decoded="$(decode_status "$sys_st")"

    local ft_sig=""
    local sys_sig=""
    if [[ "$ft_decoded" == signal:* ]]; then
        ft_sig="${ft_decoded#signal:}"
    fi
    if [[ "$sys_decoded" == signal:* ]]; then
        sys_sig="${sys_decoded#signal:}"
    fi

    local out_same="NO"
    if diff -u "$ft_norm" "$sys_norm" >"$TMP_DIR/diff_${total}.txt"; then
        out_same="YES"
        ((same_output+=1))
    fi

    [[ "$ft_st" == "$sys_st" ]] && ((same_exit+=1))
    [[ "$ft_sig" == "$sys_sig" ]] && ((same_signal+=1))
    [[ "$ft_sig" == "11" ]] && ((segfault_ft+=1))
    [[ "$sys_sig" == "11" ]] && ((segfault_sys+=1))

    printf "\n[%02d] %s\n" "$total" "$title"
    printf "ARGS:"
    printf " %q" "${args[@]}"
    printf "\n"
    printf "  ft_ping : %s\n" "$ft_decoded"
    printf "  ping    : %s\n" "$sys_decoded"
    printf "  output match (normalized): %s\n" "$out_same"

    if [[ "$out_same" == "NO" ]]; then
        echo "  ---- output diff (first 40 lines) ----"
        sed -n '1,40p' "$TMP_DIR/diff_${total}.txt"
    fi
}

echo "[INFO] ft_ping      : $FT_PING_BIN"
echo "[INFO] system ping  : $SYS_PING_BIN"
echo "[INFO] timeout      : ${TIMEOUT_DURATION}s"
echo "[INFO] sudo mode    : $RUN_SUDO"
echo "[INFO] date         : $(date '+%F %T')"

{
    "$SYS_PING_BIN" --version 2>/dev/null || "$SYS_PING_BIN" -V 2>/dev/null || true
} | sed -n '1,2p' | sed 's/^/[INFO] system ver  : /'

set -e

while IFS='|' read -r title argv; do
    [[ -z "$title" ]] && continue
    ((total+=1))
    read -r -a args <<<"$argv"
    run_case "$title" "${args[@]}"
done <<'TESTS'
basic-localhost|127.0.0.1 -c 1
basic-name|localhost -c 1
verbose-localhost|-v 127.0.0.1 -c 1
help-short|-?
help-long|--help
version-short|-V
version-long|--version
unknown-short-flag|-z 127.0.0.1
empty-flag|-
triple-hyphen|---
double-arg-error|127.0.0.1 extra
missing-target|-v
bad-hostname|notarealhost.invalid -c 1
bad-ip|300.300.300.300 -c 1
reserved-net|192.0.2.123 -c 1
long-flag-verbose|--verbose 127.0.0.1 -c 1
long-flag-address|--address 127.0.0.1
long-flag-mask|--mask 127.0.0.1
multi-help-conflict|-? --help -V
flag-burst-1|-v -v -v localhost -c 1
flag-burst-2|-? -? -? -? -?
stress-many-args|-v localhost a b c d e f g h
TESTS

echo
echo "========== SUMMARY =========="
echo "Total cases                  : $total"
echo "Same raw exit status         : $same_exit/$total"
echo "Same signal class            : $same_signal/$total"
echo "Same normalized output       : $same_output/$total"
echo "ft_ping segfaults (SIGSEGV)  : $segfault_ft"
echo "ping segfaults (SIGSEGV)     : $segfault_sys"

if (( segfault_ft > 0 )); then
    echo "[ALERT] ft_ping a segfault au moins une fois."
    exit 2
fi

exit 0