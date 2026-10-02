#include "codexion.h"

int	request_has_fifo_priority(const t_request *first,
		const t_request *second)
{
	if (first->sequence != second->sequence)
		return (first->sequence < second->sequence);
	return (first->coder_id < second->coder_id);
}

int	request_has_edf_priority(const t_request *first,
		const t_request *second)
{
	if (first->deadline != second->deadline)
		return (first->deadline < second->deadline);
	return (request_has_fifo_priority(first, second));
}

int	request_has_priority(const t_request *first,
		const t_request *second, t_scheduler scheduler)
{
	if (scheduler == SCHEDULER_FIFO)
		return (request_has_fifo_priority(first, second));
	if (scheduler == SCHEDULER_EDF)
		return (request_has_edf_priority(first, second));
	return (0);
}
