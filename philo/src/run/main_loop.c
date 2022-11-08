/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_loop.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 17:09:14 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 17:09:14 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	any_philosopher_starved(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];

		mutex_lock(&philosopher->time_of_last_meal_mutex);
		if (get_time() - philosopher->time_of_last_meal > data->time_to_die)
		{
			mutex_unlock(&philosopher->time_of_last_meal_mutex);
			print_event(EVENT_DIED, philosopher);
			return (true);
		}
		mutex_unlock(&philosopher->time_of_last_meal_mutex);

		philosopher_index++;
	}
	return (false);
}

void	main_loop(t_data *data)
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
