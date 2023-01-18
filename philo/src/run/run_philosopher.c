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

static void	run_single_philosopher_edgecase(t_philosopher *philosopher,
				t_data *data)
{
	if (should_stop(data))
		return ;
	mutex_lock(philosopher->left_fork);
	print_event(EVENT_FORK, philosopher, data);
	while (true)
	{
		if (should_stop(data))
			break ;
		usleep(LOOP_USLEEP);
	}
	mutex_unlock(philosopher->left_fork);
}

static void	grab_forks(t_philosopher *philosopher, t_data *data)
{
	if (philosopher->index % 2 == 0)
	{
		mutex_lock(philosopher->left_fork);
		print_event(EVENT_FORK, philosopher, data);
		mutex_lock(philosopher->right_fork);
		print_event(EVENT_FORK, philosopher, data);
	}
	else
	{
		mutex_lock(philosopher->right_fork);
		print_event(EVENT_FORK, philosopher, data);
		mutex_lock(philosopher->left_fork);
		print_event(EVENT_FORK, philosopher, data);
	}
}

void	eat(t_philosopher *philosopher, t_data *data)
{
	philosopher->times_eaten++;
	if (philosopher->times_eaten == data->times_to_eat)
	{
		mutex_lock(&data->philosophers_eating_mutex);
		data->philosophers_eating--;
		mutex_unlock(&data->philosophers_eating_mutex);
	}
	print_event(EVENT_EAT, philosopher, data);
	mutex_lock(&philosopher->time_of_last_meal_mutex);
	philosopher->time_of_last_meal = get_time_ms();
	mutex_unlock(&philosopher->time_of_last_meal_mutex);
	precise_sleep(philosopher->time_of_last_meal, data->time_to_eat, data);
}

void	drop_forks(t_philosopher *philosopher)
{
	mutex_unlock(philosopher->left_fork);
	mutex_unlock(philosopher->right_fork);
}

void	ph_sleep(t_philosopher *philosopher, t_data *data)
{
	print_event(EVENT_SLEEP, philosopher, data);
	precise_sleep(get_time_ms(), data->time_to_sleep, data);
}

void	think(t_philosopher *philosopher, t_data *data)
{
	print_event(EVENT_THINK, philosopher, data);
}

static void	run_regular_philosopher(t_philosopher *philosopher, t_data *data)
{
	if (philosopher->index % 2 == 1)
		precise_sleep(philosopher->time_of_last_meal, data->time_to_eat / 2,
			data);
	while (true)
	{
		if (should_stop(data))
			return ;
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
		run_single_philosopher_edgecase(philosopher, data);
	else
		run_regular_philosopher(philosopher, data);
	return (NULL);
}
