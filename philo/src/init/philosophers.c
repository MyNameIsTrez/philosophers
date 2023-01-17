/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/17 19:19:42 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/17 19:19:42 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philosophers_time_of_last_meal(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		philosopher->time_of_last_meal = data->start_time;
		philosopher_index++;
	}
}

static bool	initialize_philosopher(t_philosopher *philosopher,
				size_t philosopher_index, t_data *data)
{
	philosopher->index = philosopher_index;
	philosopher->times_eaten = 0;
	philosopher->left_fork = &data->forks[philosopher_index];
	philosopher->right_fork = &data->forks[
		(philosopher_index + 1) % data->philosopher_count];
	philosopher->data = data;
	if (!mutex_init(&philosopher->time_of_last_meal_mutex))
		return (print_error(ERROR_SYSTEM));
	if (pthread_create(&philosopher->thread, NULL, run_philosopher,
			philosopher) != PTHREAD_SUCCESS)
	{
		mutex_lock(&data->running_program_mutex);
		data->running_program = false;
		mutex_unlock(&data->running_program_mutex);
		join_philosophers(philosopher_index, data);
		return (print_error(ERROR_SYSTEM));
	}
	return (true);
}

bool	create_philosophers(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	data->philosophers = malloc(
			data->philosopher_count * sizeof(*data->philosophers));
	if (data->philosophers == NULL)
		return (print_error(ERROR_SYSTEM));
	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		philosopher->time_of_last_meal_mutex.initialized = false;
		philosopher_index++;
	}
	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		initialize_philosopher(philosopher, philosopher_index, data);
		philosopher_index++;
	}
	return (true);
}
