/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <stdio.h>

void	log_state(t_simulation *sim, int coder_id, const char *message)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->stop_reason == STOP_NONE)
	{
		timestamp = get_elapsed_ms(sim);
		if (timestamp >= 0)
			printf("%ld %d %s\n", timestamp, coder_id + 1, message);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	log_burnout(t_simulation *sim, int coder_id)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->stop_reason == STOP_NONE)
	{
		timestamp = get_elapsed_ms(sim);
		sim->stop_reason = STOP_BURNOUT;
		if (timestamp >= 0)
			printf("%ld %d burned out\n", timestamp, coder_id + 1);
		pthread_cond_broadcast(&sim->stop_condition);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_unlock(&sim->log_mutex);
}
