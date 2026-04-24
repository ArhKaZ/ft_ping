#include "ft_ping.h"

int	bypass_hyphen(char *str) {
	int count = 0;
	while (str[count] == '-') {
		count++;
	}
	return count;
}

int	is_single_hyphen_known_flag(char *str) {
	char	known_flags[] = {'V', '?', 'v'};
	int		i = 0;

	while (i < 3) {
		if (*str == known_flags[i])
			return i;
		i++;
	}
	return -1;
}

int	is_double_hyphen_known_flag(char *str) {
	char	*known_flags[] = {"version", "help", "verbose", "usage", "address", "mask", };
	int		i = 0;

	while (i < 6) {
		if (strcmp(str, known_flags[i]) == 0)
			return i;
		i++;
	}
	return -1;
}

int	verify_and_get_flag(char *flag) {
	int	hyphen_count = bypass_hyphen(flag);

	if (hyphen_count > 2)
		return -1;
	if (hyphen_count == 1)
		return is_single_hyphen_known_flag(flag + 1);
	return is_double_hyphen_known_flag(flag + 2);
}

bool	get_flags(int argc, char **argv, flag** flags) {
	const char *options = "v?V";
	int nb_flags_active = NB_LONG_FLAG_ACTIVES;
	struct option* get_opt_options = get_flags_options();
	bool	got_an_error = false;
	while (1) {
		char flag_getted = getopt_long(argc, argv, options, get_opt_options, &nb_flags_active);
		if (flag_getted == -1)
			break;
		if (flag_getted == '?' && optopt != 0) {
			got_an_error = true;
			continue;
		}
		if (*flags != NULL) {
			(*flags)->next = create_flag(flag_getted, false, optind);
		} else {
			*flags = create_flag(flag_getted, true, optind);
		}
	}
	free(get_opt_options);
	return got_an_error;
}

addresses	*create_addresses(char *address) {
	addresses	*new_addresses = malloc(sizeof(addresses));
	new_addresses->address = address;
	new_addresses->next = NULL;
	return new_addresses;
}

addresses	*get_address(char **argv, flag *flags) {
	int		i = 1;
	addresses	*addresses_getted = NULL;

	while (argv[i] != NULL) {
		if (index_not_a_flag(i, flags)) {
			if (addresses_getted == NULL) {
				addresses_getted = create_addresses(argv[i]);
			} else {
				addresses_getted->next = create_addresses(argv[i]);
			}
		}
		i++;
	}
	return addresses_getted;
}