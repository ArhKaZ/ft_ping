#ifndef FT_PING_H
# define FT_PING_H

# define ERROR_NO_ARGS 1
# define ERROR_WRONG_ARG 2
# define ERROR_ADDRESS_NO_VALUE 3

# define ERROR_NO_ARGS_TEXT "ping: missing host operand\nping: missing host operand\n"
# define ERROR_WRONG_ARG_TEXT "ping: missing host operand\nping: missing host operand\n"
# define ERROR_ADDRESS_NO_VALUE_TEXT ""

# define VERSION_DISPLAY "ft_ping (based on GNU inetutils) 2.4 \
Copyright (C) 2022 Free Software Foundation, Inc. \
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>. \
This is free software: you are free to change and redistribute it. \
There is NO WARRANTY, to the extent permitted by law. \
Based on code written by Sergey Poznyakoff."

# define HELP_DISPLAY "Usage: ft_ping [OPTION...] HOST ... \
Send ICMP ECHO_REQUEST packets to network hosts. \
\
Options controlling ICMP request types: \
      --address              send ICMP_ADDRESS packets (root only) \
      --echo                 send ICMP_ECHO packets (default) \
      --mask                 same as --address \
\
 Options valid for all request types: \
\
  -v, --verbose              verbose output \
\
 Options valid for --echo requests: \
  -?, --help                 give this help list \
      --usage                give a short usage message \
  -V, --version              print program version \
\
Mandatory or optional arguments to long options are also mandatory or optional \
for any corresponding short options. \
\
Options marked with (root only) are available only to superuser. \
\
Report bugs to syluiset."

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netdb.h>

typedef struct  s_command_options
{
    int 	is_version;
    int 	is_verbose;
    int 	is_help;
    int 	in_parsing_error;
	int		*flag_indexes;
	int		indexes_length;
    char	**address;
}				command_options;

typedef struct  s_imcp
{
    u_int8_t    type;
    u_int8_t    code;
    u_int16_t   checksum;
    u_int16_t   id;
    u_int16_t   sequence;
}                   imcp;

void display_errors(int error_type);

command_options	*flags_specified(char **args);

command_options *initialize_command_options();

char    **new_tab_plus_one(char **tab);

int	length_char_tab(char **tab);

void	launch_all_loops(char **address);

#endif
