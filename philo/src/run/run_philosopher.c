/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_philosopher.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 16:12:40 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 16:12:40 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	run_single_philosopher(t_philosopher *philosopher,
				t_data *data)
{
	if (should_stop(data))
		return ;
	mutex_lock(philosopher->left_fork);
	print_event(EVENT_FORK, philosopher, data);
	while (!should_stop(data))
		usleep(LOOP_USLEEP);
	mutex_unlock(philosopher->left_fork);
}

static void	run_regular_philosopher(t_philosopher *philosopher, t_data *data)
{
	if (philosopher->index % 2 == 1)
		precise_sleep(philosopher->time_of_last_meal, data->time_to_eat / 2,
			data);
	while (!should_stop(data))
	{
		grab_forks(philosopher, data);
		eat(philosopher, data);
		drop_forks(philosopher);
		ph_sleep(philosopher, data);
		think(philosopher, data);
		if (data->philosopher_count % 2 == 1)
			precise_sleep(get_time_ms(), data->time_to_eat / 2, data);
	}
}

void	*run_philosopher(void *_philosopher)
{
	t_philosopher	*philosopher;
	t_data			*data;

	philosopher = _philosopher;
	data = philosopher->data;
	while (true)
	{
		mutex_lock(&data->running_philosophers_mutex);
		if (data->running_philosophers)
		{
			mutex_unlock(&data->running_philosophers_mutex);
			break ;
		}
		mutex_unlock(&data->running_philosophers_mutex);
		usleep(LOOP_USLEEP);
	}
	if (data->philosopher_count == 1)
		run_single_philosopher(philosopher, data);
	else
		run_regular_philosopher(philosopher, data);
	return (NULL);
}
