#include "codexion.h"

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