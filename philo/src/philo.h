/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:12:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/18 17:12:18 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>

# define LOOP_USLEEP 1000 // TODO: Better value

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	size_t			index;
	size_t			time_of_last_meal; // TODO: What type should I be using here?
	pthread_mutex_t	time_of_last_meal_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	pthread_t		thread; // TODO: Do I need to even store this at all?
}	t_philosopher;

typedef struct s_data
{
	bool			running;
	pthread_mutex_t	running_mutex;

	pthread_mutex_t	printf_mutex;

	size_t			start_time;

	size_t			philosopher_count;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			number_of_times_each_philosopher_must_eat;

	bool			running_philosophers;
	pthread_mutex_t	running_philosophers_mutex;

	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
}	t_data;

typedef enum s_event
{
	EVENT_FORK,
	EVENT_EAT,
	EVENT_SLEEP,
	EVENT_THINK,
	EVENT_DIED,
}	t_event;

bool	ph_atoi_safe(const char *str, int *nbr);

#endif
