#include "codexion.h"

static void	*monitor_routine(void *argument)
{
	t_simulation	*sim;

	sim = (t_simulation *)argument;
	if (sim == NULL)
		return (NULL);
	return (NULL);
}

int	start_monitor(t_simulation *sim)
{
	if (sim == NULL)
		return (1);
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (1);
	return (0);
}

void	join_monitor(t_simulation *sim)
{
	if (sim == NULL)
		return ;
	pthread_join(sim->monitor_thread, NULL);
}
