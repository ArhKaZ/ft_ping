#include "ft_ping.h"

bool index_not_a_flag(int index, flag *flags) {
    for (flag *flag = flags; flag != NULL; flag = flag->next) {
        if (flags->idx == index) {
            return false;
        }
    }
    return true;
}

int get_length_flags(flag *flags) {
    int length = 0;
    for (flag *flag = flags; flag != NULL; flag = flag->next) {
        length++;
        flag = flag->next;
    }
    return length;
}

struct option create_option(int i) {
    struct option new_option;
    switch (i) {
        case 0:
            new_option.name = "verbose";
            new_option.has_arg = 0;
            new_option.val = VERBOSE_FLAG_VALUE;
            new_option.flag = NULL;
            break;
        case 1:
            new_option.name = "help";
            new_option.has_arg = 0;
            new_option.val = HELP_FLAG_VALUE;
            new_option.flag = NULL;
            break;
        case 2:
            new_option.name = "usage";
            new_option.has_arg = 0;
            new_option.val = USAGE_FLAG_VALUE;
            new_option.flag = NULL;
            break;
        case 3:
            new_option.name = "version";
            new_option.has_arg = 0;
            new_option.val = VERSION_FLAG_VALUE;
            new_option.flag = NULL;
            break;
        default:
            break;
    }
    return new_option;
}

struct option* get_flags_options() {
    struct option *options;
    int i = 0;
    options = malloc(sizeof(struct option) * NB_LONG_FLAG_ACTIVES);
    while (i < NB_LONG_FLAG_ACTIVES) {
        options[i] = create_option(i);
        i++;
    }
    return options;
}

flag    *create_flag(char flag_value, bool is_first, int idx) {
    flag *new_flag = malloc(sizeof(flag));
    new_flag->next = NULL;
    switch (flag_value) {
        case 'v':
            new_flag->name = 'v';
            new_flag->is_prio = true;
            break;
        case 'V':
            new_flag->name = 'V';
            new_flag->is_prio = true;
            break;
        case '?':
            new_flag->name = '?';
            new_flag->is_prio = true;
            break;
        case VERBOSE_FLAG_VALUE:
            new_flag->name = 'v';
            new_flag->is_prio = false;
            break;
        case HELP_FLAG_VALUE:
            new_flag->name = '?';
            new_flag->is_prio = true;
            break;
        case USAGE_FLAG_VALUE:
            new_flag->name = 'u';
            new_flag->is_prio = true;
            break;
        case VERSION_FLAG_VALUE:
            new_flag->name = 'V';
            new_flag->is_prio = true;
            break;
        default:
            free(new_flag);
            new_flag = NULL;
            break;
    }
    if (new_flag != NULL) {
        new_flag->is_first = is_first;
        new_flag->idx = idx;
    }
    return new_flag;
}

void    free_flags(flag **flags) {
    flag *next = NULL;
    if (*flags == NULL) {
        return;
    }
    while ((*flags)->next != NULL) {
        next = (*flags)->next;
        free(*flags);
        *flags = next;
    }
    free(*flags);
    *flags = NULL;
}

void    free_addresses(addresses **addr) {
    addresses   *next = NULL;
    if (*addr == NULL) {
        return;
    }
    while ((*addr)->next != NULL) {
        next = (*addr)->next;
        free(*addr);
        *addr = next;
    }
    free(*addr);
    *addr = NULL;
}