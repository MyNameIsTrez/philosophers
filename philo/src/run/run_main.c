/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_main.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 15:03:44 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 15:03:44 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philosopher_starved(t_philosopher *philosopher)
{
	size_t	time_of_last_meal;
	t_data	*data;

	data = philosopher->data;

	// TODO: This is a race condition since time_of_last_meal may change during this function
	mutex_lock(&philosopher->time_of_last_meal_mutex);
	time_of_last_meal = philosopher->time_of_last_meal;
	mutex_unlock(&philosopher->time_of_last_meal_mutex);

	return (get_time() - time_of_last_meal > data->time_to_die);
}

static bool	any_philosopher_starved(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];

		if (philosopher_starved(philosopher))
		{
			print_event(EVENT_DIED, philosopher);
			return (true);
		}

		philosopher_index++;
	}
	return (false);
}

void	run_main(t_data *data)
{
	while (true)
	{
		if (any_philosopher_starved(data))
			return ;
		mutex_lock(&data->philosophers_eating_mutex);
		if (data->philosophers_eating == 0)
		{
			mutex_unlock(&data->philosophers_eating_mutex);
			return ;
		}
		mutex_unlock(&data->philosophers_eating_mutex);
		usleep(LOOP_USLEEP);
	}
}
