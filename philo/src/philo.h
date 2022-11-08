/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:12:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 12:14:58 by sbos          ########   odam.nl         */
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

# include "error/ph_error.h"

# define LOOP_USLEEP 1000 // TODO: Better value

typedef struct s_data	t_data;

typedef struct s_mutex
{
	pthread_mutex_t	mutex;
	bool			initialized;
}	t_mutex;

typedef struct s_philosopher
{
	size_t		index;
	size_t		times_eaten;
	size_t		time_of_last_meal; // TODO: What type should I be using here?
	t_mutex		time_of_last_meal_mutex;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
	t_data		*data;
	pthread_t	thread; // TODO: Do I need to even store this at all?
}	t_philosopher;

typedef struct s_data
{
	bool			running;
	t_mutex			running_mutex;

	t_mutex			printf_mutex;

	size_t			start_time;

	size_t			philosopher_count;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			times_to_eat;

	size_t			philosopher_count_eating;
	t_mutex			philosopher_count_eating_mutex;

	bool			running_philosophers;
	t_mutex			running_philosophers_mutex;

	t_philosopher	*philosophers;
	t_mutex			*forks;
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
