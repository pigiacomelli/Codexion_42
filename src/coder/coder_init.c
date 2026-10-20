/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <stdlib.h>

static void	destroy_initialized_coders(t_simulation *sim, int count)
{
	while (count > 0)
	{
		count--;
		pthread_mutex_destroy(&sim->coders[count].mutex);
	}
	free(sim->coders);
	sim->coders = NULL;
}

static void	set_coder_fields(t_simulation *sim, int index)
{
	sim->coders[index].id = index;
	sim->coders[index].last_compile_start = sim->start_time;
	sim->coders[index].compile_count = 0;
	sim->coders[index].left_dongle = &sim->dongles[index];
	sim->coders[index].right_dongle = &sim->dongles[(index + 1)
		% sim->config.coder_count];
	sim->coders[index].simulation = sim;
}

int	init_coders(t_simulation *sim)
{
	int	index;

	if (sim == NULL || sim->config.coder_count <= 0
		|| sim->coders != NULL || sim->dongles == NULL)
		return (1);
	sim->coders = malloc(sizeof(*sim->coders) * sim->config.coder_count);
	if (sim->coders == NULL)
		return (1);
	index = 0;
	while (index < sim->config.coder_count)
	{
		set_coder_fields(sim, index);
		if (pthread_mutex_init(&sim->coders[index].mutex, NULL) != 0)
		{
			destroy_initialized_coders(sim, index);
			return (1);
		}
		index++;
	}
	return (0);
}

void	destroy_coders(t_simulation *sim)
{
	if (sim == NULL || sim->coders == NULL)
		return ;
	destroy_initialized_coders(sim, sim->config.coder_count);
}
