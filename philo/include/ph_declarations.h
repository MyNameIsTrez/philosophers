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

typedef size_t					t_time;

typedef struct s_data			t_data;
typedef struct s_mutex			t_mutex;
typedef struct s_philosopher	t_philosopher;
typedef struct s_data			t_data;

void	destroy(t_data *data);

void	mutex_lock(t_mutex *mutex_ptr);
void	mutex_unlock(t_mutex *mutex_ptr);
void	mutex_destroy(t_mutex *mutex_ptr);
bool	mutex_init(t_mutex *mutex_ptr);

bool	print_error(t_error ph_error);
void	print_event(t_event event, t_philosopher *philosopher);

size_t	ph_strlen(const char *str);

void	join_philosophers(size_t count, t_data *data);

t_time	get_time(void);
void	precise_sleep(t_philosopher *philosopher, t_time start_time,
			t_time duration);

void	*run_philosopher(void *arg);
void	main_loop(t_data *data);

bool	get_size(const char *str, size_t *size_ptr);
bool	init(int argc, char *argv[], t_data *data);

#endif
