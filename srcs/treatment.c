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

//Demande a resoudre l'hostname (www.google.com) pour avoir l'ip a la quel il faut parler
//L'assigne le type et l'addresse de l'ip a la quel on va parler
int get_info_from_hostname(char *address, struct sockaddr_in *sock_addr) {
    char buffer[1024];
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *rp;
    int             res_get_info = 0;
    memset(&buffer, 0, sizeof(buffer));
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;

    res_get_info = getaddrinfo(address, NULL, &hints, &result);
    if (res_get_info != 0) {
        printf("ERROR getaddrinfo as failed: %s\n", gai_strerror(res_get_info));
        return -1;
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {
            memcpy(sock_addr, rp->ai_addr, sizeof(struct sockaddr_in));
        }
        freeaddrinfo(result);
        return 0;
    }
    printf("ERROR no address IPV4\n");
    freeaddrinfo(result);
    
    return -1;
}

int set_ttl(int socket, int ttl) {
    return setsockopt(socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl));
}

int set_receive_timeout(int socket, int timeout_ms) {
    int seconds = timeout_ms / 1000;
    int useconds = (timeout_ms - (seconds * 1000)) * 1000;
    struct timeval tv_out;
    memset(&tv_out, 0, sizeof(tv_out));
    tv_out.tv_sec = seconds;
    tv_out.tv_usec = useconds;
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
}

u_int16_t calculate_checksum(const ping_pkt* packet) {
    int i = 0;
    int len = sizeof(ping_pkt);
    u_int16_t word = 0;
    const u_int8_t *data = (const u_int8_t *)packet;
    u_int32_t sum = 0;

    while (i < len - 1)
    {
        word = (data[i] << 8) + data[i + 1];
        sum += word;
        i += 2;
        word = 0;
    }
    if (len % 2 != 0) {
        word = data[i] << 8;
        sum += word;
    }

    sum = (sum >> 16) + (sum & 0XFFFF);
    sum += (sum >> 16);
    return (u_int16_t)~sum;
}

void    initialize_icmp_packet(ping_pkt *packet, int sequence, pid_t pid) {
    long unsigned int i = 0;

    memset(packet, 0, sizeof(*packet));
    packet->hdr.type = 8;
    packet->hdr.code = 0;
    packet->hdr.echo.id = htons(pid);
    packet->hdr.echo.sequence = htons(sequence);

    while (i < ICMP_PAYLOAD_LENGTH) {
        packet->payload[i] = (char)('0' + i);
        i++;
    }
    packet->hdr.checksum = 0;
    u_int16_t calc = calculate_checksum((const ping_pkt *) packet);
    packet->hdr.checksum = htons(calc);
}

info_package_sended send_package(int fd, ping_pkt* packet, struct sockaddr_in* addr, int pid) {
    int done = 0;
    int data_received_length = 0;
    char    buffer[1024];
    int ip_header_length = 20;
    int min_icmp_response_size = ip_header_length + 8;
    ping_pkt *data_received;
    ssize_t send_to_length = 0;
    socklen_t from_len = sizeof(*addr);
    info_package_sended info_to_return;

    clock_gettime(CLOCK_MONOTONIC, &info_to_return.starttime);

    send_to_length = sendto(fd, packet, sizeof(*packet), 0, (struct sockaddr* )addr, sizeof(*addr));
    if (send_to_length < 0) {
        printf("ERROR sendto");
        return info_to_return;
    }
    while (!done) {
        from_len = sizeof(*addr);
        data_received_length = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)addr, &from_len);
        clock_gettime(CLOCK_MONOTONIC, &info_to_return.endtime);
        if (data_received_length < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("TIMEOUT\n");
                return info_to_return;
            }
            printf("RECVFROM ERROR");
            return info_to_return;
        }
        if (data_received_length < min_icmp_response_size) {
            continue;
        }
        data_received = (ping_pkt*)&buffer[ip_header_length];
        if (verify_reply(data_received, pid)) {
            printf("GOT A PACKET\n");
            done = 1;
        } else {
            printf("GOT SOMETHING ELSE\n");
        }
    }
    return info_to_return;
}

int verify_reply(ping_pkt *received, int expected_id) {
    if (received->hdr.type != 0) {
        return 0;
    }
    if (received->hdr.code != 0) {
        return 0;
    }
    if (received->hdr.echo.id != htons(expected_id)) {
        return 0;
    }
    return 1;
}

void    display_info_package(ping_pkt *packet, info_package_sended *infos_returned) {
    (void)packet;
    if (infos_returned == NULL) {
        return;
    }
    
}

int launch_send_to(int sequence, struct sockaddr_in *dest_addr) {
    
    int fd = 0;
    ping_pkt    packet;
    info_package_sended infos_returned;

    memset(&infos_returned, 0, sizeof(infos_returned));
    
    fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd < 0) {
        printf("%s : %s", "ERROR while creating socket\n", strerror(errno));
        return -1;
    }
    if (set_ttl(fd, 64) != 0) {
        printf("%s", "ERROR with set ttl\n");
        return -1;
    }
    if (set_receive_timeout(fd, 2000) != 0) {
        printf("%s", "ERROR with set receive timeout\n");
        return -1;
    }
    pid_t pid = getpid();
    initialize_icmp_packet(&packet, sequence, pid);
    infos_returned = send_package(fd, &packet, dest_addr, pid);
    display_info_package(&packet, &infos_returned);
    return 0;
}

void    launch_all_loops(char **addresses) {
    int i = 0;
    struct sockaddr_in dest_addr;
    set_signal_action();
    while (addresses[i] != NULL) {
        is_cancelled = 0;
        int j = 0;
        memset(&dest_addr, 0, sizeof(dest_addr));
        if (get_info_from_hostname(addresses[i], &dest_addr) < 0) {
            return ;
        }
        printf("PING %s (%s)\n", addresses[i], "0.0.0.0");
        while (!is_cancelled) {
            launch_send_to(j, &dest_addr);
            j++;
            sleep(1000);
        }
        i++;
    }
}