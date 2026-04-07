#include "ft_ping.h"

int treat_info_flags(command_options *cmd_options) {
    if (cmd_options->is_help) {
        printf("%s", HELP_DISPLAY);
        return 1;
    }
    if (cmd_options->is_version) {
        printf("%s", VERSION_DISPLAY);
        return 1;
    }
    return 0;
}

int launch_send_to(char *address) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in dest_addr;
    socklen_t dest_len;
    imcp    *packet;
    char    *data;

    if (fd < 0) {
        printf("%s", "error while creating socket");
        return -1;
    }
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(address);
    getnameinfo();
    gethostbyaddr
    data = packet + 8;
    memset(data, 'A', 56);
    packet->checksum = 
    sendto
}

void    launch_all_loops(char **addresses) {
    int i = 0;
    
    while (addresses[i] != NULL) {
        launch_send_to(addresses[i]);
    }
}