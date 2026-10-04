/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 16:16:07 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap_internal.h"

#include <stdlib.h>

int	heap_init(t_heap *heap, size_t capacity, t_scheduler scheduler)
{
	if (heap == NULL)
		return (1);
	heap->requests = NULL;
	heap->size = 0;
	heap->capacity = 0;
	heap->scheduler = scheduler;
	if (capacity == 0 || (scheduler != SCHEDULER_FIFO
			&& scheduler != SCHEDULER_EDF))
		return (1);
	if (capacity > (size_t)-1 / sizeof(*heap->requests))
		return (1);
	heap->requests = malloc(sizeof(*heap->requests) * capacity);
	if (heap->requests == NULL)
		return (1);
	heap->capacity = capacity;
	return (0);
}

void	heap_destroy(t_heap *heap)
{
	if (heap == NULL)
		return ;
	free(heap->requests);
	heap->requests = NULL;
	heap->size = 0;
	heap->capacity = 0;
}

static int	heap_grow(t_heap *heap)
{
	t_request	**new_requests;
	size_t		new_capacity;
	size_t		index;

	if (heap->capacity > (size_t)-1 / 2)
		return (1);
	new_capacity = heap->capacity * 2;
	if (new_capacity > (size_t)-1 / sizeof(*new_requests))
		return (1);
	new_requests = malloc(sizeof(*new_requests) * new_capacity);
	if (new_requests == NULL)
		return (1);
	index = 0;
	while (index < heap->size)
	{
		new_requests[index] = heap->requests[index];
		index++;
	}
	free(heap->requests);
	heap->requests = new_requests;
	heap->capacity = new_capacity;
	return (0);
}

int	heap_push(t_heap *heap, t_request *request)
{
	if (heap == NULL || request == NULL || heap->requests == NULL)
		return (1);
	if (heap->size > heap->capacity)
		return (1);
	if (heap->size == heap->capacity && heap_grow(heap) != 0)
		return (1);
	heap->requests[heap->size] = request;
	heap->size++;
	heap_sift_up(heap, heap->size - 1);
	return (0);
}
