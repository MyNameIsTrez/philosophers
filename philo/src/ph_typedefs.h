/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_typedefs.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 15:16:30 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 15:16:30 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_TYPEDEFS_H
# define PH_TYPEDEFS_H

typedef enum s_event
{
	EVENT_FORK,
	EVENT_EAT,
	EVENT_SLEEP,
	EVENT_THINK,
	EVENT_DIED,
}	t_event;

typedef struct s_mutex
{
	pthread_mutex_t	mutex;
	bool			initialized;
}	t_mutex;

typedef struct s_data	t_data;
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

	size_t			philosophers_eating;
	t_mutex			philosophers_eating_mutex;

	bool			running_philosophers;
	t_mutex			running_philosophers_mutex;

	t_philosopher	*philosophers;
	t_mutex			*forks;
}	t_data;

#endif
