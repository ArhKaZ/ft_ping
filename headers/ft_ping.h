#ifndef FT_PING_H
# define FT_PING_H

# define ERROR_NO_ARGS 1
# define ERROR_WRONG_ARG 2
# define ERROR_ADDRESS_NO_VALUE 3

# define ERROR_NO_ARGS_TEXT "ping: missing host operand\nping: missing host operand\n"
# define ERROR_WRONG_ARG_TEXT "ping: missing host operand\nping: missing host operand\n"
# define ERROR_ADDRESS_NO_VALUE_TEXT ""

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

typedef struct  s_command_options
{
    int is_version;
    int is_verbose;
    int is_help;
    int in_error;
    int got_address;
}           command_options;


void display_errors(int error_type);

command_options	*flags_specified(char **args);

command_options *initialize_command_options();

#endif
