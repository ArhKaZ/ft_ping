#include "ft_ping.h"

void	display_errors(int error_type) {
	char	*error = NULL;

	switch (error_type)
	{
		case ERROR_NO_ARGS:
			error = strdup(ERROR_NO_ARGS_TEXT);
			break;
		case ERROR_WRONG_ARG:
			error = strdup(ERROR_WRONG_ARG_TEXT);
		default:
			break;
	}
	printf("%s", error);
}