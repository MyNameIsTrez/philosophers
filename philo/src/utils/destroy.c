/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   destroy.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 14:59:21 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 14:59:21 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_data *data)
{
	size_t			fork_index;

	if (data->forks == NULL)
		return ;
	fork_index = 0;
	while (fork_index < data->philosopher_count && data->forks[fork_index].initialized)
	{
		mutex_destroy(&data->forks[fork_index]);
		fork_index++;
	}
}

static void	destroy_philosophers(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	if (data->philosophers == NULL)
		return ;
	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count
	&& data->philosophers[philosopher_index].time_of_last_meal_mutex.initialized)
	{
		philosopher = &data->philosophers[philosopher_index];
		mutex_destroy(&philosopher->time_of_last_meal_mutex);
		philosopher_index++;
	}
}

void	destroy(t_data *data)
{
	mutex_destroy(&data->running_program_mutex);
	mutex_destroy(&data->printf_mutex);
	mutex_destroy(&data->philosophers_eating_mutex);
	mutex_destroy(&data->running_philosophers_mutex);

	destroy_forks(data);
	free(data->forks);

	destroy_philosophers(data);
	free(data->philosophers);
}
