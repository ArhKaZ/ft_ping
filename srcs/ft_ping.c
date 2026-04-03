#include "ft_ping.h"

int main(int argc, char **argv) {
    command_options *cmd_options;
    if (argc < 2) {
        display_errors(ERROR_NO_ARGS);
        return 64;
    }
    cmd_options = flags_selected(argv);
}