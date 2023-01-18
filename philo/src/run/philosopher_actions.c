/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosopher_actions.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/18 15:28:45 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/18 15:28:45 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	grab_forks(t_philosopher *philosopher, t_data *data)
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
