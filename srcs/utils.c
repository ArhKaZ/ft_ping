#include "ft_ping.h"

command_options *initialize_command_options() {
    command_options *cmd_options;

    cmd_options = malloc(sizeof(command_options));
    cmd_options->address = NULL;
    cmd_options->in_parsing_error = 0;
    cmd_options->is_help = 0;
    cmd_options->is_verbose = 0;
    cmd_options->is_version = 0;
    cmd_options->address = NULL;
    cmd_options->flag_indexes = NULL;

    return cmd_options;
}

int index_not_in_tab(int index, int *tab, int length) {
    int i = 0;

    while (i < length) {
        if (index == tab[i])
            return 0;
    }
    return 1;
}

char    **clean_args(char **argv, command_options *cmd_options) {
    int i = 0;
    char **new_argv = NULL;

    new_argv = malloc(sizeof(argv) * (cmd_options->indexes_length + 1));
    while (argv[i + 1] != NULL) {
        if (index_not_in_tab(i, cmd_options->flag_indexes, cmd_options->indexes_length)) {
            new_argv[i] = strdup(argv[i + 1]);
        }
        i++;
    }
    argv[i + 1] = NULL;
    return new_argv;
}

int	length_char_tab(char **tab) {
	int i = 0;

    if (tab != NULL) {
	    while (tab[i] != NULL) {
	    	i++;
        }
    }
	return i;
}

char    **new_tab_plus_one(char **tab) {
    int length = length_char_tab(tab);
    char **new_tab = malloc(sizeof(char *) * (length + 1));
    int i = 0;

    if (tab == NULL) {
        new_tab[i] = strdup("\0");
    } else {
        while (i < length) {
            new_tab[i] = strdup(tab[i]);
            i++;
        }
    }
    return new_tab;
}