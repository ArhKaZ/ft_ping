#include "ft_ping.h"

int main(int argc, char **argv) {
    command_options *cmd_options;
    const char *options = "v?V";
    int nb_flags_active = NB_LONG_FLAG_ACTIVES;
    if (argc < 2) {
        display_errors(ERROR_NO_ARGS);
        return 64;
    }
    const struct option* get_opt_options = get_flags_options();
    while (1) {
        char flag_getted = getopt_long(argc, argv, options, get_opt_options, &nb_flags_active);
        printf("%d", flag_getted);
        //STOCKER LES FLAG DEDANS
        //VOIR COMMENT NE PLUS AVOIR LES FLAGS DANS ARGV DU COUP
        if (flag_getted == -1)
            break;
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
    char **clean_argv = clean_args(argv, cmd_options);
    get_address(clean_argv, &cmd_options);
    launch_all_loops(cmd_options->address);
    return 0;
}