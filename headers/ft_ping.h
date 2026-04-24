#ifndef FT_PING_H
# define FT_PING_H

# define ERROR_NO_ARGS 1
# define ERROR_WRONG_ARG 2
# define ERROR_ADDRESS_NO_VALUE 3

# define ERROR_NO_ARGS_TEXT "ft_ping: missing host operand\nTry 'ft_ping --help' or 'ft_ping --usage' for more information.\n"
# define ERROR_WRONG_ARG_TEXT "ft_ping: invalid option\nTry 'ft_ping --help' or 'ft_ping --usage' for more information.\n"
# define ERROR_ADDRESS_NO_VALUE_TEXT ""

# define VERSION_DISPLAY "ft_ping (based on GNU inetutils) 2.4 \n Copyright (C) 2022 Free Software Foundation, Inc. \n License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>. \n This is free software: you are free to change and redistribute it. \n There is NO WARRANTY, to the extent permitted by law. \n Based on code written by Sergey Poznyakoff.\n"
# define HELP_DISPLAY "Usage: ft_ping [OPTION...] HOST ... \n Send ICMP ECHO_REQUEST packets to network hosts. \n\n Options controlling ICMP request types: \n      --address              send ICMP_ADDRESS packets (root only) \n      --echo                 send ICMP_ECHO packets (default) \n      --mask                 same as --address \n\n Options valid for all request types: \n\n  -v, --verbose              verbose output \n\n Options valid for --echo requests: \n  -?, --help                 give this help list \n      --usage                give a short usage message \n  -V, --version              print program version \n\nMandatory or optional arguments to long options are also mandatory or optional \nfor any corresponding short options. \n\nOptions marked with (root only) are available only to superuser. \n\nReport bugs to syluiset.\n"
# define USAGE_DISPLAY "Usage: ping [-v?V]\n            [--address] [--echo]\n            [--verbose]\n            [--quiet] [--help] [--usage] [--version]\n            HOST ...\n"

# define ICMP_PAYLOAD_LENGTH (64 - sizeof(icmp))

# define NB_LONG_FLAG_ACTIVES 4
# define VERBOSE_FLAG_VALUE 0
# define HELP_FLAG_VALUE 1
# define USAGE_FLAG_VALUE 2
# define VERSION_FLAG_VALUE 3

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <time.h>
# include <getopt.h>

extern volatile sig_atomic_t is_cancelled;

typedef struct s_flag {
    char	        name;
    bool	        is_first;
    bool            is_prio;
    int             idx;
    struct s_flag   *next;
}                       flag;

typedef struct  s_echo {
    u_int16_t   id;
    u_int16_t   sequence;
}               echo;

typedef struct  s_icmp
{
    u_int8_t    type;
    u_int8_t    code;
    u_int16_t   checksum;
    echo        echo;
}                   icmp;

typedef struct s_ping_pkt
{
    icmp hdr;
    char payload[ICMP_PAYLOAD_LENGTH];
}               ping_pkt;

typedef struct s_info_package_sended {
    struct timespec starttime;
    struct timespec endtime;
    int             ttl;
}               info_package_sended;

typedef struct s_addresses {
    char    *address;
    struct s_addresses *next;
}               addresses;

void display_errors(int error_type);

void    launch_all_loops(addresses *addresses);

int	verify_reply(ping_pkt *received, int expected_id);

int treat_info_flags(flag *flags);

addresses	*get_address(char **argv, flag *flags);

void    set_signal_action(void);

struct option* get_flags_options();

flag    *create_flag(char flag_value, bool is_first, int idx);

bool	get_flags(int argc, char **argv, flag **flags);

void    free_flags(flag **flags);

void    free_addresses(addresses **addr);

bool index_not_a_flag(int index, flag *flags);

void    free_options(struct option *options);

#endif
