#include "codexion.h"

#include <assert.h>
#include <stdio.h>

static t_request	make_request(int coder_id, unsigned long sequence,
		long deadline)
{
	t_request	request;

	request.coder_id = coder_id;
	request.sequence = sequence;
	request.deadline = deadline;
	request.state = REQUEST_PENDING;
	return (request);
}

static void	test_fifo(void)
{
	t_request	first;
	t_request	second;

	first = make_request(2, 10, 900);
	second = make_request(1, 20, 100);
	assert(request_has_fifo_priority(&first, &second) == 1);
	assert(request_has_fifo_priority(&second, &first) == 0);
	second.sequence = first.sequence;
	assert(request_has_fifo_priority(&second, &first) == 1);
}

static void	test_edf(void)
{
	t_request	first;
	t_request	second;

	first = make_request(2, 20, 100);
	second = make_request(1, 10, 900);
	assert(request_has_edf_priority(&first, &second) == 1);
	second.deadline = first.deadline;
	assert(request_has_edf_priority(&second, &first) == 1);
	assert(request_has_priority(&first, &second, SCHEDULER_FIFO) == 0);
	assert(request_has_priority(&first, &second, SCHEDULER_EDF) == 0);
}

static void	test_heap_lifecycle(void)
{
	t_heap	heap;

	assert(heap_init(&heap, 4, SCHEDULER_FIFO) == 0);
	assert(heap.requests != NULL);
	assert(heap.size == 0);
	assert(heap.capacity == 4);
	assert(heap.scheduler == SCHEDULER_FIFO);
	heap_destroy(&heap);
	assert(heap.requests == NULL);
	assert(heap.size == 0);
	assert(heap.capacity == 0);
	assert(heap_init(&heap, 0, SCHEDULER_EDF) == 1);
	assert(heap.requests == NULL);
}

int	main(void)
{
	test_fifo();
	test_edf();
	test_heap_lifecycle();
	printf("heap comparison tests: OK\n");
	return (0);
}
