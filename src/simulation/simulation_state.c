#include "codexion.h"

int	simulation_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = (sim->stop_reason != STOP_NONE);
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

void	stop_simulation(t_simulation *sim, t_stop_reason reason)
{
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->stop_reason == STOP_NONE && reason != STOP_NONE)
	{
		sim->stop_reason = reason;
		pthread_cond_broadcast(&sim->stop_condition);
	}
	pthread_mutex_unlock(&sim->state_mutex);
}
