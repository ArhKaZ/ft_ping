#include "ft_ping.h"

int	bypass_hyphen(char *str) {
	int count = 0;
	while (str[count] == '-') {
		count++;
	}
	return count;
}

int	is_single_hyphen_known_flag(char *str) {
	char	known_flags[] = {'v', '?', 'v'};
	int		i = 0;

	while (i < 3) {
		if (str == known_flags[i])
			return i;
		i++;
	}
	return -1;
}

int	is_double_hyphen_known_flag(char *str) {
	char	*known_flags[] = {"version", "help", "verbose", "address", "mask"};
	int		i = 0;

	while (i < 5) {
		if (strcmp(str, known_flags[i]) == 0)
			return ;
		i++;
	}
	return -1;
}

int	verify_and_get_flag(char *flag) {
	int	hyphen_count = bypass_hyphen(flag);
	int	result = 0;

	if (hyphen_count > 2)
		return -1;
	if (hyphen_count == 1)
		return is_single_hyphen_known_flag(flag + 1);
	else
		return is_double_hyphen_known_flag(flag + 2);

}

void	fill_options(int flag_type, command_options **cmd_options) {
	switch (flag_type)
	{
		case -1:
			(*cmd_options)->in_parsing_error = ERROR_WRONG_ARG;
			break;
		case 0:
			(*cmd_options)->is_version = 1;
			break;
		case 1:
			(*cmd_options)->is_help = 1;
			break;
		case 2:
			(*cmd_options)->is_verbose = 1;
			break;
		default:
			break;
	}
}

int	update_indexes(int **flag_indexes, int current_length, int new_index) {
	int	new_length = current_length + 1;
	int	*old_indexes = malloc(sizeof(int) * current_length);
	int i = 0;

	while (i < current_length) {
		old_indexes[i] = *flag_indexes[i];
		i++;
	}

	free(*flag_indexes);
	flag_indexes = malloc(sizeof(int) * new_length);
	i = 0;
	while (i < old_indexes) {
		flag_indexes[i] = old_indexes[i];
		i++;
	}
	flag_indexes[++i] = new_index;
	return new_length;
}

command_options	*flags_specified(char **args) {
	int i = 0;
	int	flag_type = 0;
	int	*flag_find_indexes = NULL;
	int	flag_indexes_length = 0;
	command_options *cmd_options;

	cmd_options = initialize_command_options();

	while (args[i] != NULL) {
		if (strchr(args[i], '-')) {
			flag_type = verify_flag(args[i]);
			fill_options(flag_type, &cmd_options);
			flag_find_indexes = update_indexes(&flag_find_indexes, flag_find_indexes, i);
		}
	}
	cmd_options->flag_indexes = flag_find_indexes;
	cmd_options->indexes_length = flag_find_indexes;
	return cmd_options;
}


void	get_address(char **argv, command_options **command_options) {
	int		i = 0;

	while (argv[i] != NULL) {
		char	**new_tab_address = copy_tab_plus_one((*command_options)->address);
		int		current_length = length_char_tab(new_tab_address);
		new_tab_address[current_length + 1] = strdup(argv[i]);
		free((*command_options)->address);
		(*command_options)->address = new_tab_address;
		i++;
	}
}