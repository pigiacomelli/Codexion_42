#include "codexion.h"

#include <sys/time.h>

long	get_time_ms(void)
{
	struct timeval	now;

	if (gettimeofday(&now, 0) != 0)
		return (-1);
	return (now.tv_sec * 1000L + now.tv_usec / 1000L);
}

long	get_elapsed_ms(t_simulation *sim)
{
	long	current_time;

	current_time = get_time_ms();
	if (current_time < 0)
		return (-1);
	return (current_time - sim->start_time);
}
