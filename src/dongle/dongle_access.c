/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle_internal.h"

#include <limits.h>

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	if (dongle == NULL || dongle_prepare_request(&request, coder) != 0)
		return (1);
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return (1);
	request.sequence = dongle->next_sequence++;
	if (heap_push(&dongle->waiting_requests, &request) != 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	if (dongle_wait_for_request(coder, dongle, &request) != 0)
		return (1);
	log_state(coder->simulation, coder->id, "has taken a dongle");
	return (0);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	long	current_time;
	long	cooldown;

	if (coder == NULL || coder->simulation == NULL || dongle == NULL)
		return ;
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return ;
	if (dongle->owner_id != coder->id)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	current_time = get_time_ms();
	cooldown = coder->simulation->config.dongle_cooldown;
	dongle->owner_id = -1;
	if (current_time < 0 || cooldown > LONG_MAX - current_time)
		dongle->available_at = LONG_MAX;
	else
		dongle->available_at = current_time + cooldown;
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
	if (current_time < 0)
		stop_simulation(coder->simulation, STOP_ERROR);
}
