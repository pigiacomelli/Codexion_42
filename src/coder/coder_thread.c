/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_internal.h"

static void	*coder_routine(void *argument)
{
	t_coder	*coder;

	coder = (t_coder *)argument;
	if (coder == NULL)
		return (NULL);
	if (coder->left_dongle == coder->right_dongle)
	{
		coder_run_single(coder);
		return (NULL);
	}
	while (!simulation_stopped(coder->simulation)
		&& !coder_finished(coder))
	{
		if (coder_run_cycle(coder) != 0)
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
