#include "codexion.h"

#include <limits.h>
#include <string.h>

static int	parse_long(const char *text, long *number)
{
	long	value;
	int		digit;
	int		index;

	if (text == 0 || text[0] == '\0')
		return (1);
	value = 0;
	index = 0;
	while (text[index] != '\0')
	{
		if (text[index] < '0' || text[index] > '9')
			return (1);
		digit = text[index] - '0';
		if (value > (LONG_MAX - digit) / 10)
			return (1);
		value = value * 10 + digit;
		index++;
	}
	*number = value;
	return (0);
}

static int	parse_numbers(char **arguments, long *values)
{
	int	index;

	index = 0;
	while (index < 7)
	{
		if (parse_long(arguments[index + 1], &values[index]) != 0)
			return (1);
		index++;
	}
	return (0);
}

static int	store_config(long *values, t_config *config)
{
	if (values[0] == 0 || values[0] > INT_MAX
		|| values[5] > INT_MAX)
		return (1);
	config->coder_count = (int)values[0];
	config->burnout_time = values[1];
	config->compile_time = values[2];
	config->debug_time = values[3];
	config->refactor_time = values[4];
	config->required_compiles = (int)values[5];
	config->dongle_cooldown = values[6];
	return (0);
}

static int	parse_scheduler(const char *text, t_scheduler *scheduler)
{
	if (strcmp(text, "fifo") == 0)
		*scheduler = SCHEDULER_FIFO;
	else if (strcmp(text, "edf") == 0)
		*scheduler = SCHEDULER_EDF;
	else
		return (1);
	return (0);
}

int	parse_arguments(int argc, char **argv, t_config *config)
{
	long	values[7];

	if (argc != 9 || config == 0)
		return (1);
	if (parse_numbers(argv, values) != 0)
		return (1);
	if (store_config(values, config) != 0)
		return (1);
	return (parse_scheduler(argv[8], &config->scheduler));
}
