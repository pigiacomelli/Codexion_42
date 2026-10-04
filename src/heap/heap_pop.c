/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 16:16:07 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 16:16:07 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <stdlib.h>

static void	heap_sift_down(t_heap *heap, size_t index)
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

t_request	*heap_peek(const t_heap *heap)
{
	if (heap == NULL || heap->requests == NULL || heap->size == 0)
		return (NULL);
	return (heap->requests[0]);
}

t_request	*heap_pop(t_heap *heap)
{
	t_request	*top_request;

	if (heap == NULL || heap->requests == NULL || heap->size == 0)
		return (NULL);
	top_request = heap->requests[0];
	heap->size--;
	if (heap->size > 0)
		heap->requests[0] = heap->requests[heap->size];
	heap->requests[heap->size] = NULL;
	if (heap->size > 0)
		heap_sift_down(heap, 0);
	return (top_request);
}
