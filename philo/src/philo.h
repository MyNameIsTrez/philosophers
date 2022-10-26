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

# include "types.h"

typedef struct s_data
{
	bool	running;
}	t_data;

typedef struct s_philosopher
{
	size_t			index;
	t_f64			ms_time_of_last_meal; // TODO: What type should I be using here?
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	pthread_t		thread; // TODO: Do I need to even store this at all?
}	t_philosopher;

typedef enum s_event
{
	EVENT_FORK,
	EVENT_SLEEP,
	EVENT_EAT,
}	t_event;

bool	ph_atoi_safe(const char *str, t_i32 *nbr);

#endif
