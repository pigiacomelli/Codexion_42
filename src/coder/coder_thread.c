#include "codexion.h"

static int	update_compile_start(t_coder *coder)
{
	long	current_time;

	current_time = get_time_ms();
	if (current_time < 0)
		return (1);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = current_time;
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}

static void	mark_compile_completed(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
}

static int	coder_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->mutex);
	finished = (coder->compile_count
			>= coder->simulation->config.required_compiles);
	pthread_mutex_unlock(&coder->mutex);
	return (finished);
}

static int	run_coder_cycle(t_coder *coder)
{
	int		sleep_result;

	if (acquire_dongle_pair(coder) != 0)
		return (1);
	if (update_compile_start(coder) != 0)
	{
		release_dongle_pair(coder);
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	log_state(coder->simulation, coder->id, "is compiling");
	sleep_result = interruptible_sleep(coder->simulation,
			coder->simulation->config.compile_time);
	release_dongle_pair(coder);
	if (sleep_result < 0)
	{
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	if (sleep_result > 0)
		return (1);
	mark_compile_completed(coder);
	log_state(coder->simulation, coder->id, "is debugging");
	sleep_result = interruptible_sleep(coder->simulation,
		coder->simulation->config.debug_time);
	if (sleep_result < 0)
	{
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	if (sleep_result > 0)
		return (1);
	log_state(coder->simulation, coder->id, "is refactoring");
	sleep_result = interruptible_sleep(coder->simulation,
		coder->simulation->config.refactor_time);
	if (sleep_result < 0)
	{
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	if (sleep_result > 0)
		return (1);
	return (0);
}

static void	*coder_routine(void *argument)
{
	t_coder	*coder;

	coder = (t_coder *)argument;
	if (coder == NULL)
		return (NULL);
	while (!simulation_stopped(coder->simulation)
		&& !coder_finished(coder))
	{
		if (run_coder_cycle(coder) != 0)
			break ;
	}
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
