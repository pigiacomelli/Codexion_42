/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_internal.h"

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

int	coder_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->mutex);
	finished = (coder->compile_count
			>= coder->simulation->config.required_compiles);
	pthread_mutex_unlock(&coder->mutex);
	return (finished);
}

static int	run_timed_state(t_coder *coder, const char *message, long duration)
{
	int	result;

	log_state(coder->simulation, coder->id, message);
	result = interruptible_sleep(coder->simulation, duration);
	if (result < 0)
	{
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	return (result > 0);
}

int	coder_run_cycle(t_coder *coder)
{
	if (acquire_dongle_pair(coder) != 0)
		return (1);
	if (update_compile_start(coder) != 0)
	{
		release_dongle_pair(coder);
		stop_simulation(coder->simulation, STOP_ERROR);
		return (1);
	}
	if (run_timed_state(coder, "is compiling",
			coder->simulation->config.compile_time) != 0)
	{
		release_dongle_pair(coder);
		return (1);
	}
	release_dongle_pair(coder);
	mark_compile_completed(coder);
	if (run_timed_state(coder, "is debugging",
			coder->simulation->config.debug_time) != 0)
		return (1);
	return (run_timed_state(coder, "is refactoring",
			coder->simulation->config.refactor_time));
}
