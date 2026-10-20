/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor_internal.h"

int	monitor_all_coders_finished(t_simulation *sim)
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

int	monitor_check_burnouts(t_simulation *sim)
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
