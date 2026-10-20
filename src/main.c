/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;

	if (parse_arguments(argc, argv, &config) != 0)
		return (1);
	if (init_simulation(&simulation, &config) != 0)
		return (1);
	if (start_coders(&simulation) != 0)
	{
		destroy_simulation(&simulation);
		return (1);
	}
	if (start_monitor(&simulation) != 0)
	{
		stop_simulation(&simulation, STOP_ERROR);
		join_coders(&simulation);
		destroy_simulation(&simulation);
		return (1);
	}
	join_monitor(&simulation);
	join_coders(&simulation);
	destroy_simulation(&simulation);
	return (0);
}
