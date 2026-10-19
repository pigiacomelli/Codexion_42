/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 15:34:26 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/05 17:02:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <stdlib.h>

static int	init_one_dongle(t_dongle *dongle,
		size_t index, t_simulation *sim)
{
	dongle->id = (int)index;
	dongle->owner_id = -1;
	dongle->available_at = 0;
	dongle->next_sequence = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (1);
	if (pthread_cond_init(&dongle->condition, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (1);
	}
	if (heap_init(&dongle->waiting_requests,
			(size_t)sim->config.coder_count,
			sim->config.scheduler) != 0)
	{
		pthread_cond_destroy(&dongle->condition);
		pthread_mutex_destroy(&dongle->mutex);
		return (1);
	}
	return (0);
}

static void	destroy_initialized_dongles(t_simulation *sim,
		size_t initialized_count)
{
	while (initialized_count > 0)
	{
		initialized_count--;
		heap_destroy(&sim->dongles[initialized_count].waiting_requests);
		pthread_cond_destroy(&sim->dongles[initialized_count].condition);
		pthread_mutex_destroy(&sim->dongles[initialized_count].mutex);
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

int	init_dongles(t_simulation *sim)
{
	size_t	count;
	size_t	index;

	if (sim == NULL || sim->config.coder_count <= 0)
		return (1);
	sim->dongles = NULL;
	count = (size_t)sim->config.coder_count;
	if (count > (size_t)-1 / sizeof(*sim->dongles))
		return (1);
	sim->dongles = malloc(sizeof(*sim->dongles) * count);
	if (sim->dongles == NULL)
		return (1);
	index = 0;
	while (index < count)
	{
		if (init_one_dongle(&sim->dongles[index], index, sim) != 0)
		{
			destroy_initialized_dongles(sim, index);
			return (1);
		}
		index++;
	}
	return (0);
}

void	destroy_dongles(t_simulation *sim)
{
	if (sim == NULL || sim->dongles == NULL)
		return ;
	destroy_initialized_dongles(sim,
		(size_t)sim->config.coder_count);
}
