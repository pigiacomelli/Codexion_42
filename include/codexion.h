#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stddef.h>

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

int		request_has_fifo_priority(const t_request *first,
			const t_request *second);
int		request_has_edf_priority(const t_request *first,
			const t_request *second);
int		request_has_priority(const t_request *first,
			const t_request *second, t_scheduler scheduler);
int		heap_init(t_heap *heap, size_t capacity, t_scheduler scheduler);
void	heap_destroy(t_heap *heap);
void	log_state(t_simulation *sim, int coder_id, const char *message);
void	log_burnout(t_simulation *sim, int coder_id);
int		parse_arguments(int argc, char **argv, t_config *config);
int		init_simulation(t_simulation *sim, const t_config *config);
void	destroy_simulation(t_simulation *sim);
long	get_time_ms(void);
long	get_elapsed_ms(t_simulation *sim);
int		interruptible_sleep(t_simulation *sim, long duration_ms);
int		simulation_stopped(t_simulation *sim);
void	stop_simulation(t_simulation *sim, t_stop_reason reason);

#endif
