/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heap_compare.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/04 16:16:07 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include <assert.h>
#include <stdio.h>

void	test_heap_remove_sift_up(void);
void	test_heap_stress(void);

static void	test_comparisons(void)
{
	t_request	first;
	t_request	second;

	first = (t_request){2, 10, 900, REQUEST_PENDING};
	second = (t_request){1, 20, 100, REQUEST_PENDING};
	assert(request_has_fifo_priority(&first, &second) == 1);
	assert(request_has_fifo_priority(&second, &first) == 0);
	second.sequence = first.sequence;
	assert(request_has_fifo_priority(&second, &first) == 1);
	first = (t_request){2, 20, 100, REQUEST_PENDING};
	second = (t_request){1, 10, 900, REQUEST_PENDING};
	assert(request_has_edf_priority(&first, &second) == 1);
	second.deadline = first.deadline;
	assert(request_has_edf_priority(&second, &first) == 1);
	assert(request_has_priority(&first, &second, SCHEDULER_FIFO) == 0);
	assert(request_has_priority(&first, &second, SCHEDULER_EDF) == 0);
}

static void	test_heap_lifecycle(void)
{
	t_request	request;
	t_heap		heap;

	request = (t_request){1, 10, 100, REQUEST_PENDING};
	assert(heap_init(&heap, 4, SCHEDULER_FIFO) == 0);
	assert(heap.requests != NULL);
	assert(heap.size == 0);
	assert(heap.capacity == 4);
	assert(heap.scheduler == SCHEDULER_FIFO);
	assert(heap_peek(&heap) == NULL);
	assert(heap_pop(&heap) == NULL);
	assert(heap_push(&heap, &request) == 0);
	assert(heap_remove(&heap, &request) == 0 && heap.size == 0);
	heap_destroy(&heap);
	assert(heap.requests == NULL);
	assert(heap.size == 0);
	assert(heap.capacity == 0);
	assert(heap_init(&heap, 0, SCHEDULER_EDF) == 1);
	assert(heap.requests == NULL);
}

static void	test_fifo_heap(void)
{
	t_request	requests[4];
	t_heap		heap;

	requests[0] = (t_request){1, 40, 100, REQUEST_PENDING};
	requests[1] = (t_request){2, 10, 400, REQUEST_PENDING};
	requests[2] = (t_request){3, 30, 200, REQUEST_PENDING};
	requests[3] = (t_request){4, 20, 300, REQUEST_PENDING};
	assert(heap_init(&heap, 2, SCHEDULER_FIFO) == 0);
	assert(heap_push(&heap, &requests[0]) == 0);
	assert(heap_push(&heap, &requests[1]) == 0);
	assert(heap_push(&heap, &requests[2]) == 0);
	assert(heap_push(&heap, &requests[3]) == 0);
	assert(heap.size == 4 && heap.capacity == 4);
	assert(heap_peek(&heap)->sequence == 10 && heap.size == 4);
	assert(heap_remove(&heap, &requests[3]) == 0 && heap.size == 3);
	assert(heap_remove(&heap, &requests[3]) == 1);
	assert(heap_pop(&heap)->sequence == 10);
	assert(heap_pop(&heap)->sequence == 30);
	assert(heap_pop(&heap)->sequence == 40);
	assert(heap_pop(&heap) == NULL && heap_peek(&heap) == NULL);
	heap_destroy(&heap);
}

static void	test_edf_heap(void)
{
	t_request	requests[5];
	t_heap		heap;

	requests[0] = (t_request){3, 30, 300, REQUEST_PENDING};
	requests[1] = (t_request){2, 20, 100, REQUEST_PENDING};
	requests[2] = (t_request){4, 40, 300, REQUEST_PENDING};
	requests[3] = (t_request){5, 10, 100, REQUEST_PENDING};
	requests[4] = (t_request){1, 10, 100, REQUEST_PENDING};
	assert(heap_init(&heap, 1, SCHEDULER_EDF) == 0);
	assert(heap_push(&heap, &requests[0]) == 0);
	assert(heap_push(&heap, &requests[1]) == 0);
	assert(heap_push(&heap, &requests[2]) == 0);
	assert(heap_push(&heap, &requests[3]) == 0);
	assert(heap_push(&heap, &requests[4]) == 0);
	assert(heap.size == 5 && heap.capacity == 8);
	assert(heap_remove(&heap, &requests[4]) == 0 && heap.size == 4);
	assert(heap_pop(&heap) == &requests[3]);
	assert(heap_pop(&heap) == &requests[1]);
	assert(heap_pop(&heap) == &requests[0]);
	assert(heap_pop(&heap) == &requests[2]);
	assert(heap_pop(&heap) == NULL);
	heap_destroy(&heap);
}

int	main(void)
{
	test_comparisons();
	test_heap_lifecycle();
	test_fifo_heap();
	test_edf_heap();
	test_heap_remove_sift_up();
	test_heap_stress();
	printf("heap tests: OK\n");
	return (0);
}
