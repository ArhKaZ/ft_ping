#include "ft_ping.h"

int main(int argc, char **argv) {
    command_options *cmd_options;
    char            **clean_argv;
    
    if (argc < 2) {
        display_errors(ERROR_NO_ARGS);
        return 64;
    }
    cmd_options = flags_specified(argv);
    if (cmd_options->in_parsing_error) {
        display_errors(cmd_options->in_parsing_error);
        free(cmd_options);
        return 64;
    }
    if (treat_info_flags(cmd_options)) {
        free(cmd_options);
        return 0;
    }
    clean_argv = clean_args(argv, cmd_options);
    get_address(clean_argv, &cmd_options);
    launch_all_loops(cmd_options->address);
    return 0;
}