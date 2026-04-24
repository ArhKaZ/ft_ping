#include "ft_ping.h"

int main(int argc, char **argv) {
    flag    *flags = NULL;
    if (argc < 2) {
        display_errors(ERROR_NO_ARGS);
        return 64;
    }
    if (get_flags(argc, argv, &flags)) {
        return -1; // TODO: VOIR CODE ERREUR WRONG PARAMETERS
    }
    if (treat_info_flags(flags)) {
        free_flags(&flags);
        return 0;
    }
    addresses *addresses = get_address(argv, flags);
    launch_all_loops(addresses);
    free_flags(&flags);
    free_addresses(&addresses);
    return 0;
}