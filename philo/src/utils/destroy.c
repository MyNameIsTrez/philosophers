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

static void	destroy_forks(size_t count, t_data *data)
{
	size_t			fork_index;

	if (data->forks == NULL)
		return ;
	fork_index = 0;
	while (fork_index < count)
	{
		mutex_destroy(&data->forks[fork_index]);
		fork_index++;
	}
}

static void	destroy_philosophers(size_t count, t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	if (data->philosophers == NULL)
		return ;
	philosopher_index = 0;
	while (philosopher_index < count)
	{
		philosopher = &data->philosophers[philosopher_index];
		mutex_destroy(&philosopher->time_of_last_meal_mutex);
		philosopher_index++;
	}
}

void	destroy(t_data *data)
{
	mutex_destroy(&data->running_mutex);
	mutex_destroy(&data->printf_mutex);
	mutex_destroy(&data->philosophers_eating_mutex);
	mutex_destroy(&data->running_philosophers_mutex);

	destroy_forks(data->philosopher_count, data);
	free(data->forks);

	destroy_philosophers(data->philosopher_count, data);
	free(data->philosophers);
}
