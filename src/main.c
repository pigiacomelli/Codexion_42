#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;

	if (parse_arguments(argc, argv, &config) != 0)
		return (1);
	if (init_simulation(&simulation, &config) != 0)
		return (1);
	destroy_simulation(&simulation);
	return (0);
}
