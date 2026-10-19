#include "codexion.h"

#include <errno.h>
#include <limits.h>
#include <time.h>

static int	prepare_request(t_request *request, t_coder *coder)
{
	long	last_compile_start;
	long	burnout_time;

	if (request == NULL || coder == NULL || coder->simulation == NULL)
		return (1);
	request->coder_id = coder->id;
	request->state = REQUEST_PENDING;
	if (pthread_mutex_lock(&coder->mutex) != 0)
		return (1);
	last_compile_start = coder->last_compile_start;
	burnout_time = coder->simulation->config.burnout_time;
	if (burnout_time > LONG_MAX - last_compile_start)
		request->deadline = LONG_MAX;
	else
		request->deadline = last_compile_start + burnout_time;
	if (pthread_mutex_unlock(&coder->mutex) != 0)
		return (1);
	return (0);
}

static int	wait_for_dongle(t_dongle *dongle)
{
	struct timespec	deadline;
	long			wake_time;
	int				result;

	wake_time = get_time_ms();
	if (wake_time < 0 || wake_time > LONG_MAX - 5)
		return (1);
	wake_time += 5;
	deadline.tv_sec = wake_time / 1000;
	deadline.tv_nsec = (wake_time % 1000) * 1000000L;
	result = pthread_cond_timedwait(&dongle->condition,
			&dongle->mutex, &deadline);
	if (result != 0 && result != ETIMEDOUT)
		return (1);
	return (0);
}

static int	request_can_be_granted(t_dongle *dongle,
		t_request *request)
{
	long	current_time;

	if (dongle->owner_id != -1
		|| heap_peek(&dongle->waiting_requests) != request)
		return (0);
	current_time = get_time_ms();
	if (current_time < 0 || current_time < dongle->available_at)
		return (0);
	return (1);
}

static int	cancel_request(t_coder *coder, t_dongle *dongle,
		t_request *request, int internal_error)
{
	request->state = REQUEST_CANCELLED;
	heap_remove(&dongle->waiting_requests, request);
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
	if (internal_error)
		stop_simulation(coder->simulation, STOP_ERROR);
	return (1);
}

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	if (dongle == NULL || prepare_request(&request, coder) != 0)
		return (1);
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return (1);
	request.sequence = dongle->next_sequence++;
	if (heap_push(&dongle->waiting_requests, &request) != 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	while (!request_can_be_granted(dongle, &request))
	{
		if (simulation_stopped(coder->simulation))
			return (cancel_request(coder, dongle, &request, 0));
		if (wait_for_dongle(dongle) != 0)
			return (cancel_request(coder, dongle, &request, 1));
	}
	if (simulation_stopped(coder->simulation))
		return (cancel_request(coder, dongle, &request, 0));
	if (heap_pop(&dongle->waiting_requests) != &request)
		return (cancel_request(coder, dongle, &request, 1));
	request.state = REQUEST_GRANTED;
	dongle->owner_id = coder->id;
	pthread_mutex_unlock(&dongle->mutex);
	log_state(coder->simulation, coder->id, "has taken a dongle");
	return (0);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	long	current_time;
	long	cooldown;

	if (coder == NULL || coder->simulation == NULL || dongle == NULL)
		return ;
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return ;
	if (dongle->owner_id != coder->id)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	current_time = get_time_ms();
	cooldown = coder->simulation->config.dongle_cooldown;
	dongle->owner_id = -1;
	if (current_time < 0 || cooldown > LONG_MAX - current_time)
		dongle->available_at = LONG_MAX;
	else
		dongle->available_at = current_time + cooldown;
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
	if (current_time < 0)
		stop_simulation(coder->simulation, STOP_ERROR);
}
