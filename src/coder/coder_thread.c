#include "codexion.h"

static void	*coder_routine(void *argument)
{
	(void)argument;
	return (NULL);
}

static void	join_started_coders(t_simulation *sim, int started_count)
{
	while (started_count > 0)
	{
		started_count--;
		pthread_join(sim->coders[started_count].thread, NULL);
	}
}

int	start_coders(t_simulation *sim)
{
	int	index;

	if (sim == NULL || sim->coders == NULL)
		return (1);
	index = 0;
	while (index < sim->config.coder_count)
	{
		if (pthread_create(&sim->coders[index].thread, NULL,
				coder_routine, &sim->coders[index]) != 0)
		{
			stop_simulation(sim, STOP_ERROR);
			join_started_coders(sim, index);
			return (1);
		}
		index++;
	}
	return (0);
}

void	join_coders(t_simulation *sim)
{
	int	index;

	if (sim == NULL || sim->coders == NULL)
		return ;
	index = 0;
	while (index < sim->config.coder_count)
	{
		pthread_join(sim->coders[index].thread, NULL);
		index++;
	}
}
