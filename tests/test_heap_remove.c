/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heap_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 17:04:42 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 17:04:42 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <assert.h>

static void	fill_remove_heap(t_heap *heap, t_request *requests)
{
	unsigned long	sequences[7];
	size_t			index;

	sequences[0] = 1;
	sequences[1] = 10;
	sequences[2] = 2;
	sequences[3] = 11;
	sequences[4] = 12;
	sequences[5] = 3;
	sequences[6] = 4;
	index = 0;
	while (index < 7)
	{
		requests[index] = (t_request){(int)index + 1,
			sequences[index], 100, REQUEST_PENDING};
		assert(heap_push(heap, &requests[index]) == 0);
		index++;
	}
}

void	test_heap_remove_sift_up(void)
{
	t_request		requests[7];
	unsigned long	expected[6];
	t_heap			heap;
	size_t			index;

	expected[0] = 1;
	expected[1] = 2;
	expected[2] = 3;
	expected[3] = 4;
	expected[4] = 10;
	expected[5] = 11;
	assert(heap_init(&heap, 7, SCHEDULER_FIFO) == 0);
	fill_remove_heap(&heap, requests);
	assert(heap_remove(&heap, &requests[4]) == 0);
	index = 0;
	while (index < 6)
	{
		assert(heap_pop(&heap)->sequence == expected[index]);
		index++;
	}
	assert(heap_pop(&heap) == NULL);
	heap_destroy(&heap);
}
