#include "codexion.h"

static int	all_coders_finished(t_simulation *sim)
{
	int	index;
	int	finished;

	index = 0;
	while (index < sim->config.coder_count)
	{
		pthread_mutex_lock(&sim->coders[index].mutex);
		finished = (sim->coders[index].compile_count
				>= sim->config.required_compiles);
		pthread_mutex_unlock(&sim->coders[index].mutex);
		if (!finished)
			return (0);
		index++;
	}
	return (1);
}

static int	check_coder_burnout(t_simulation *sim, int index)
{
	t_coder	*coder;
	long	current_time;
	long	elapsed;
	int		finished;

	coder = &sim->coders[index];
	pthread_mutex_lock(&coder->mutex);
	finished = (coder->compile_count >= sim->config.required_compiles);
	current_time = get_time_ms();
	if (current_time < 0)
	{
		pthread_mutex_unlock(&coder->mutex);
		return (-1);
	}
	elapsed = current_time - coder->last_compile_start;
	if (!finished && elapsed >= sim->config.burnout_time)
	{
		log_burnout(sim, coder->id);
		pthread_mutex_unlock(&coder->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}

static int	check_burnouts(t_simulation *sim)
{
	int	index;
	int	result;

	index = 0;
	while (index < sim->config.coder_count)
	{
		result = check_coder_burnout(sim, index);
		if (result != 0)
			return (result);
		index++;
	}
	return (0);
}

static void	*monitor_routine(void *argument)
{
	t_simulation	*sim;
	int				result;

	sim = (t_simulation *)argument;
	if (sim == NULL)
		return (NULL);
	while (!simulation_stopped(sim))
	{
		if (all_coders_finished(sim))
		{
			stop_simulation(sim, STOP_SUCCESS);
			return (NULL);
		}
		result = check_burnouts(sim);
		if (result != 0)
		{
			if (result < 0)
				stop_simulation(sim, STOP_ERROR);
			return (NULL);
		}
		if (interruptible_sleep(sim, 1) < 0)
		{
			stop_simulation(sim, STOP_ERROR);
			return (NULL);
		}
	}
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
