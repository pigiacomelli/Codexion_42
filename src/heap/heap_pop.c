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

#include "heap_internal.h"

#include <stdlib.h>

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

static size_t	heap_find(const t_heap *heap, const t_request *request)
{
	size_t	index;

	index = 0;
	while (index < heap->size && heap->requests[index] != request)
		index++;
	return (index);
}

static void	heap_restore(t_heap *heap, size_t index)
{
	size_t	parent_index;

	if (index > 0)
	{
		parent_index = (index - 1) / 2;
		if (request_has_priority(heap->requests[index],
				heap->requests[parent_index], heap->scheduler))
		{
			heap_sift_up(heap, index);
			return ;
		}
	}
	heap_sift_down(heap, index);
}

int	heap_remove(t_heap *heap, t_request *request)
{
	size_t	index;

	if (heap == NULL || heap->requests == NULL || request == NULL)
		return (1);
	index = heap_find(heap, request);
	if (index == heap->size)
		return (1);
	heap->size--;
	if (index != heap->size)
		heap->requests[index] = heap->requests[heap->size];
	heap->requests[heap->size] = NULL;
	if (index != heap->size)
		heap_restore(heap, index);
	return (0);
}
