/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor_internal.h"

static void	*monitor_routine(void *argument)
{
	t_simulation	*sim;
	int				result;

	sim = (t_simulation *)argument;
	if (sim == NULL)
		return (NULL);
	while (!simulation_stopped(sim))
	{
		if (monitor_all_coders_finished(sim))
		{
			stop_simulation(sim, STOP_SUCCESS);
			return (NULL);
		}
		result = monitor_check_burnouts(sim);
		if (result != 0)
		{
			if (result < 0)
				stop_simulation(sim, STOP_ERROR);
			return (NULL);
		}
		if (interruptible_sleep(sim, 1) < 0)
			stop_simulation(sim, STOP_ERROR);
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
