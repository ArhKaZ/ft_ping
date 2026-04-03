#include "ft_ping.h"

command_options *initialize_command_options() {
    command_options *cmd_options;

    cmd_options = malloc(sizeof(command_options));
    cmd_options->got_address = 0;
    cmd_options->in_error = 0;
    cmd_options->is_help = 0;
    cmd_options->is_verbose = 0;
    cmd_options->is_version = 0;

    return cmd_options;
}