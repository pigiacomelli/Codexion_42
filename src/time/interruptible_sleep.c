#include "codexion.h"

#include <errno.h>
#include <time.h>

static int	build_deadline(struct timespec *deadline, long duration_ms)
{
	if (clock_gettime(CLOCK_REALTIME, deadline) != 0)
		return (1);
	deadline->tv_sec += duration_ms / 1000;
	deadline->tv_nsec += (duration_ms % 1000) * 1000000L;
	if (deadline->tv_nsec >= 1000000000L)
	{
		deadline->tv_sec++;
		deadline->tv_nsec -= 1000000000L;
	}
	return (0);
}

int	interruptible_sleep(t_simulation *sim, long duration_ms)
{
	struct timespec	deadline;
	int				wait_result;
	int				interrupted;

	if (duration_ms < 0 || build_deadline(&deadline, duration_ms) != 0)
		return (-1);
	if (pthread_mutex_lock(&sim->state_mutex) != 0)
		return (-1);
	wait_result = 0;
	while (sim->stop_reason == STOP_NONE && wait_result == 0)
		wait_result = pthread_cond_timedwait(&sim->stop_condition,
				&sim->state_mutex, &deadline);
	if (wait_result != 0 && wait_result != ETIMEDOUT)
	{
		pthread_mutex_unlock(&sim->state_mutex);
		return (-1);
	}
	interrupted = (sim->stop_reason != STOP_NONE);
	if (pthread_mutex_unlock(&sim->state_mutex) != 0)
		return (-1);
	return (interrupted);
}
