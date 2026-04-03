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
			(*cmd_options)->in_error = ERROR_WRONG_ARG;
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


command_options	*flags_specified(char **args) {
	int i = 0;
	int	flag_type = 0;
	command_options *cmd_options;

	cmd_options = initialize_command_options();

	while (args[i] != NULL) {
		if (strchr(args[i], '-')) {
			flag_type = verify_flag(args[i]);
			fill_options(flag_type, &cmd_options);
			if (cmd_options->in_error || cmd_options->is_help || cmd_options->is_version || cmd_options->is_verbose)
				break;
		}
	}
	return cmd_options;
}