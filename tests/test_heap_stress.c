/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heap_stress.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 17:26:32 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 17:26:32 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <assert.h>

#define STRESS_SIZE 64

static void	init_stress_requests(t_request *requests)
{
	size_t	index;

	index = 0;
	while (index < STRESS_SIZE)
	{
		requests[index].coder_id = (int)index + 1;
		requests[index].sequence = (index * 37) % STRESS_SIZE;
		requests[index].deadline = 100 + (index * 53) % 13;
		requests[index].state = REQUEST_PENDING;
		index++;
	}
}

static void	assert_heap_property(const t_heap *heap)
{
	size_t	index;
	size_t	parent_index;

	index = 1;
	while (index < heap->size)
	{
		parent_index = (index - 1) / 2;
		assert(!request_has_priority(heap->requests[index],
				heap->requests[parent_index], heap->scheduler));
		index++;
	}
}

static void	remove_stress_samples(t_heap *heap, t_request *requests)
{
	assert(heap_remove(heap, &requests[0]) == 0);
	assert_heap_property(heap);
	assert(heap_remove(heap, &requests[17]) == 0);
	assert_heap_property(heap);
	assert(heap_remove(heap, &requests[31]) == 0);
	assert_heap_property(heap);
	assert(heap_remove(heap, &requests[63]) == 0);
	assert_heap_property(heap);
}

static void	drain_stress_heap(t_heap *heap)
{
	t_request	*previous;
	t_request	*current;

	previous = heap_pop(heap);
	while (heap->size > 0)
	{
		current = heap_pop(heap);
		assert(!request_has_priority(current, previous, heap->scheduler));
		previous = current;
		assert_heap_property(heap);
	}
	assert(heap_pop(heap) == NULL);
}

void	test_heap_stress(void)
{
	t_request	requests[STRESS_SIZE];
	t_heap		heap;
	size_t		index;
	int			scheduler;

	scheduler = SCHEDULER_FIFO;
	while (scheduler <= SCHEDULER_EDF)
	{
		init_stress_requests(requests);
		assert(heap_init(&heap, 1, (t_scheduler)scheduler) == 0);
		index = 0;
		while (index < STRESS_SIZE)
		{
			assert(heap_push(&heap, &requests[index++]) == 0);
			assert_heap_property(&heap);
		}
		remove_stress_samples(&heap, requests);
		drain_stress_heap(&heap);
		heap_destroy(&heap);
		scheduler++;
	}
}
