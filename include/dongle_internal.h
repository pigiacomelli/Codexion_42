/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/20 10:00:00 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_INTERNAL_H
# define DONGLE_INTERNAL_H

# include "codexion.h"

int	dongle_prepare_request(t_request *request, t_coder *coder);
int	dongle_wait_for_request(t_coder *coder, t_dongle *dongle,
		t_request *request);

#endif
