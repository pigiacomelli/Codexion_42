/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 17:04:42 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 17:04:42 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_INTERNAL_H
# define HEAP_INTERNAL_H

# include "codexion.h"

void	heap_sift_up(t_heap *heap, size_t index);
void	heap_sift_down(t_heap *heap, size_t index);

#endif
