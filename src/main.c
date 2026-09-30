#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;

	if (parse_arguments(argc, argv, &config) != 0)
		return (1);
	if (init_simulation(&simulation, &config) != 0)
		return (1);
	log_state(&simulation, 1, "has taken a dongle");
	log_state(&simulation, 1, "is compiling");
	log_burnout(&simulation, 2);
	log_state(&simulation, 1, "is debugging");
	destroy_simulation(&simulation);
	return (0);
}
