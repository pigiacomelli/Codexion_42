/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_order.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 17:04:42 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 17:04:42 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap_internal.h"

void	heap_sift_up(t_heap *heap, size_t index)
{
	t_request	*temporary;
	size_t		parent_index;

	while (index > 0)
	{
		parent_index = (index - 1) / 2;
		if (!request_has_priority(heap->requests[index],
				heap->requests[parent_index], heap->scheduler))
			break ;
		temporary = heap->requests[index];
		heap->requests[index] = heap->requests[parent_index];
		heap->requests[parent_index] = temporary;
		index = parent_index;
	}
}

void	heap_sift_down(t_heap *heap, size_t index)
{
	t_request	*temporary;
	size_t		left_index;
	size_t		right_index;
	size_t		priority_index;

	while (index < heap->size)
	{
		left_index = index * 2 + 1;
		right_index = left_index + 1;
		priority_index = index;
		if (left_index < heap->size
			&& request_has_priority(heap->requests[left_index],
				heap->requests[priority_index], heap->scheduler))
			priority_index = left_index;
		if (right_index < heap->size
			&& request_has_priority(heap->requests[right_index],
				heap->requests[priority_index], heap->scheduler))
			priority_index = right_index;
		if (priority_index == index)
			break ;
		temporary = heap->requests[index];
		heap->requests[index] = heap->requests[priority_index];
		heap->requests[priority_index] = temporary;
		index = priority_index;
	}
}
