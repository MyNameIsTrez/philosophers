/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 14:49:32 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 14:49:32 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			&(t_pthread_args){philosopher, data}) != PTHREAD_SUCCESS)
	{
		mutex_lock(&data->running_program_mutex);
		data->running_program = false;
		mutex_unlock(&data->running_program_mutex);
		join_philosophers(philosopher_index, data);
		return (print_error(ERROR_SYSTEM));
	}
	return (true);
}

__attribute__((warn_unused_result))
static bool	create_philosophers(t_data *data)
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

__attribute__((warn_unused_result))
static bool	init_forks(t_data *data)
{
	size_t	fork_index;

	data->forks = malloc(data->philosopher_count * sizeof(*data->forks));
	if (data->forks == NULL)
		return (print_error(ERROR_SYSTEM));
	fork_index = 0;
	while (fork_index < data->philosopher_count)
	{
		data->forks[fork_index].initialized = false;
		fork_index++;
	}
	fork_index = 0;
	while (fork_index < data->philosopher_count)
	{
		if (!mutex_init(&data->forks[fork_index]))
			return (print_error(ERROR_SYSTEM));
		fork_index++;
	}
	return (true);
}

__attribute__((warn_unused_result))
static bool	init_argv(int argc, char *argv[], t_data *data)
{
	data->times_to_eat = 0;
	if (!parse_size(argv[1], &data->philosopher_count)
		|| !parse_size(argv[2], &data->time_to_die)
		|| !parse_size(argv[3], &data->time_to_eat)
		|| !parse_size(argv[4], &data->time_to_sleep)
		|| (argc == 6 && !parse_size(argv[5], &data->times_to_eat)))
		return (print_error(ERROR_EXPECTED_NATURAL_NUMBER));
	return (true);
}

__attribute__((warn_unused_result))
static bool	init_data_mutexes(t_data *data)
{
	data->running_program_mutex.initialized = false;
	data->printf_mutex.initialized = false;
	data->philosophers_eating_mutex.initialized = false;
	data->running_philosophers_mutex.initialized = false;
	return (mutex_init(&data->running_program_mutex)
		&& mutex_init(&data->printf_mutex)
		&& mutex_init(&data->philosophers_eating_mutex)
		&& mutex_init(&data->running_philosophers_mutex));
}

static void	init_philosophers_time_of_last_meal(t_data *data)
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

bool	init(int argc, char *argv[], t_data *data)
{
	memset(data, 0, sizeof(*data));
	if (argc < 5 || argc > 6)
		return (print_error(ERROR_WRONG_ARGUMENT_COUNT));
	if (!init_data_mutexes(data))
		return (print_error(ERROR_INIT_MUTEX));
	if (!init_argv(argc, argv, data) || !init_forks(data))
		return (false);
	data->philosophers_eating = data->philosopher_count;
	data->running_program = true;
	data->running_philosophers = false;
	if (!create_philosophers(data))
		return (false);
	data->start_time = get_time_ms();
	init_philosophers_time_of_last_meal(data);
	mutex_lock(&data->running_philosophers_mutex);
	data->running_philosophers = true;
	mutex_unlock(&data->running_philosophers_mutex);
	return (true);
}
