#include "codexion.h"

int	init_simulation(t_simulation *sim, const t_config *config)
{
	sim->config = *config;
	sim->stop_reason = STOP_NONE;
	if (pthread_mutex_init(&sim->state_mutex, 0) != 0)
		return (1);
	if (pthread_mutex_init(&sim->log_mutex, 0) != 0)
	{
		pthread_mutex_destroy(&sim->state_mutex);
		return (1);
	}
	if (pthread_cond_init(&sim->stop_condition, 0) != 0)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->state_mutex);
		return (1);
	}
	sim->start_time = get_time_ms();
	if (sim->start_time < 0)
	{
		destroy_simulation(sim);
		return (1);
	}
	return (0);
}

void	destroy_simulation(t_simulation *sim)
{
	pthread_cond_destroy(&sim->stop_condition);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
}
