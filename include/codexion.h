/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pigiacom <pietrogiacomelli8@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by pigiacom          #+#    #+#             */
/*   Updated: 2026/10/05 14:54:59 by pigiacom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stddef.h>

typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef enum e_stop_reason
{
	STOP_NONE,
	STOP_SUCCESS,
	STOP_BURNOUT,
	STOP_ERROR
}	t_stop_reason;

typedef struct s_config
{
	int			coder_count;
	long		burnout_time;
	long		compile_time;
	long		debug_time;
	long		refactor_time;
	int			required_compiles;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_simulation
{
	t_config		config;
	long			start_time;
	t_stop_reason	stop_reason;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	log_mutex;
	pthread_cond_t	stop_condition;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_simulation;

typedef enum e_request_state
{
	REQUEST_PENDING,
	REQUEST_GRANTED,
	REQUEST_CANCELLED
}	t_request_state;

typedef struct s_request
{
	int				coder_id;
	unsigned long	sequence;
	long			deadline;
	t_request_state	state;
}	t_request;

typedef struct s_heap
{
	t_request	**requests;
	size_t		size;
	size_t		capacity;
	t_scheduler	scheduler;
}	t_heap;

struct s_dongle
{
	int				id;
	int				owner_id;
	long			available_at;
	unsigned long	next_sequence;
	pthread_mutex_t	mutex;
	pthread_cond_t	condition;
	t_heap			waiting_requests;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	long			last_compile_start;
	int				compile_count;
	pthread_mutex_t	mutex;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_simulation	*simulation;
};

int			request_has_fifo_priority(const t_request *first,
				const t_request *second);
int			request_has_edf_priority(const t_request *first,
				const t_request *second);
int			request_has_priority(const t_request *first,
				const t_request *second, t_scheduler scheduler);
int			heap_init(t_heap *heap, size_t capacity, t_scheduler scheduler);
void		heap_destroy(t_heap *heap);
int			heap_push(t_heap *heap, t_request *request);
t_request	*heap_peek(const t_heap *heap);
t_request	*heap_pop(t_heap *heap);
int			heap_remove(t_heap *heap, t_request *request);
void		log_state(t_simulation *sim, int coder_id, const char *message);
void		log_burnout(t_simulation *sim, int coder_id);
int			parse_arguments(int argc, char **argv, t_config *config);
int			init_simulation(t_simulation *sim, const t_config *config);
int			init_dongles(t_simulation *sim);
void		destroy_dongles(t_simulation *sim);
int			acquire_dongle(t_coder *coder, t_dongle *dongle);
void		release_dongle(t_coder *coder, t_dongle *dongle);
int			init_coders(t_simulation *sim);
void		destroy_coders(t_simulation *sim);
int			start_coders(t_simulation *sim);
void		join_coders(t_simulation *sim);
void		destroy_simulation(t_simulation *sim);
long		get_time_ms(void);
long		get_elapsed_ms(t_simulation *sim);
int			interruptible_sleep(t_simulation *sim, long duration_ms);
int			simulation_stopped(t_simulation *sim);
void		stop_simulation(t_simulation *sim, t_stop_reason reason);

#endif
