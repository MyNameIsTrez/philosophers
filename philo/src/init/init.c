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
