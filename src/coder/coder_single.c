/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_single.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_internal.h"

static int	wait_for_simulation_stop(t_simulation *sim)
{
	int	result;

	if (pthread_mutex_lock(&sim->state_mutex) != 0)
		return (1);
	result = 0;
	while (sim->stop_reason == STOP_NONE && result == 0)
		result = pthread_cond_wait(&sim->stop_condition,
				&sim->state_mutex);
	pthread_mutex_unlock(&sim->state_mutex);
	return (result != 0);
}

void	coder_run_single(t_coder *coder)
{
	if (acquire_dongle(coder, coder->left_dongle) != 0)
		return ;
	if (wait_for_simulation_stop(coder->simulation) != 0)
		stop_simulation(coder->simulation, STOP_ERROR);
	release_dongle(coder, coder->left_dongle);
}
