/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_declarations.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/17 15:26:34 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/17 15:26:34 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_DECLARATIONS_H
# define PH_DECLARATIONS_H

# include "philo.h"

typedef size_t					t_time;

typedef struct s_data			t_data;
typedef struct s_philosopher	t_philosopher;
typedef struct s_mutex			t_mutex;

// init/init.c
bool	init(int argc, char *argv[], t_data *data);

// init/parse_size.c
bool	parse_size(const char *str, size_t *size_ptr);

// init/philosophers.c
void	init_philosophers_time_of_last_meal(t_data *data);
bool	create_philosophers(t_data *data);

// run/philosopher_actions.c
void	grab_forks(t_philosopher *philosopher, t_data *data);
void	eat(t_philosopher *philosopher, t_data *data);
void	drop_forks(t_philosopher *philosopher);
void	ph_sleep(t_philosopher *philosopher, t_data *data);
void	think(t_philosopher *philosopher, t_data *data);

// run/run_philosopher.c
void	*run_philosopher(void *pthread_args);

// run/run.c
void	run(t_data *data);

// utils/destroy.c
void	destroy(t_data *data);

// utils/join_philosophers.c
void	join_philosophers(size_t count, t_data *data);

// utils/mutex.c
void	mutex_lock(t_mutex *mutex_ptr);
void	mutex_unlock(t_mutex *mutex_ptr);
void	mutex_destroy(t_mutex *mutex_ptr);
bool	mutex_init(t_mutex *mutex_ptr);

// utils/print_error.c
bool	print_error(t_error ph_error);

// utils/print_event.c
void	print_event(t_event event, t_philosopher *philosopher, t_data *data);

// utils/should_stop.c
bool	should_stop(t_data *data);

// utils/strlen.c
size_t	ph_strlen(const char *str);

// utils/time.c
t_time	get_time_ms(void);
void	precise_sleep(t_time start_time, t_time duration, t_data *data);

#endif
