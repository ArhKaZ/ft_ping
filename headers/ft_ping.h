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

typedef struct  s_command_options
{
    int 	is_version;
    int 	is_verbose;
    int 	is_help;
    int     is_usage;
    int 	in_parsing_error;
	int		*flag_indexes;
	int		indexes_length;
    char	**address;
}				command_options;

typedef struct  s_echo {
    u_int16_t   id;
    u_int16_t   sequence;
}               echo;

typedef struct  s_frag {
    u_int16_t   __unused;
    u_int16_t   mtu;
}               frag;

typedef struct  s_icmp
{
    u_int8_t    type;
    u_int8_t    code;
    u_int16_t   checksum;
    echo        echo;
    u_int32_t   gateway;
    frag        frag;
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

void display_errors(int error_type);

command_options	*flags_specified(char **args);

command_options *initialize_command_options();

char    **new_tab_plus_one(char **tab);

int	length_char_tab(char **tab);

void	launch_all_loops(char **address);

int	verify_reply(ping_pkt *received, int expected_id);

int treat_info_flags(command_options *cmd_options);

char    **clean_args(char **argv, command_options *cmd_options);

void	get_address(char **argv, command_options **command_options);

void    set_signal_action(void);

struct option* get_flags_options();

#endif
