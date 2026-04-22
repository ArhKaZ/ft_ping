#include "ft_ping.h"

volatile sig_atomic_t is_cancelled = 0;

void    sigint_handler(int signal) {
    if (signal == SIGINT) {
        is_cancelled = 1;
    }
}

void    set_signal_action(void) {
    struct sigaction act;
    bzero(&act, sizeof(act));
    act.sa_handler = &sigint_handler;
    sigaction(SIGINT, &act, NULL);
}