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

static bool	create_philosophers(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	data->philosophers = malloc(data->philosopher_count * sizeof(*data->philosophers));
	if (data->philosophers == NULL)
		return (false);
	philosopher_index = 0;

	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];

		philosopher->index = philosopher_index;

		philosopher->times_eaten = 0;

		philosopher->left_fork = &data->forks[philosopher_index];
		philosopher->right_fork = &data->forks[(philosopher_index + 1) % data->philosopher_count];

		philosopher->data = data;

		philosopher->time_of_last_meal_mutex.initialized = false;
		if (!mutex_init(&philosopher->time_of_last_meal_mutex))
			return (false);

		if (pthread_create(&philosopher->thread, NULL, run_philosopher, philosopher) != PTHREAD_SUCCESS)
		{
			mutex_lock(&data->running_program_mutex);
			data->running_program = false;
			mutex_unlock(&data->running_program_mutex);
			join_philosophers(philosopher_index, data);
			return (false);
		}

		philosopher_index++;
	}

	return (true);
}

static bool	init_forks(t_data *data)
{
	size_t	fork_index;

	fork_index = 0;
	data->forks = malloc(data->philosopher_count * sizeof(*data->forks));
	if (data->forks == NULL)
		return (false);

	while (fork_index < data->philosopher_count)
	{
		data->forks[fork_index].initialized = false;
		if (!mutex_init(&data->forks[fork_index]))
			return (false);
		fork_index++;
	}

	return (true);
}

static bool	init_argv(int argc, char *argv[], t_data *data)
{
	int	nbr;

	// TODO: Throw an error in case an arg is < 0 or <= 0?

	if (!ph_atoi_safe(argv[1], &nbr))
	{
		// TODO: Should this print an error message?
		return (false);
	}
	data->philosopher_count = (size_t)nbr;
	if (!ph_atoi_safe(argv[2], &nbr))
	{
		// TODO: Should this print an error message?
		return (false);
	}
	data->time_to_die = (t_time)nbr;
	if (!ph_atoi_safe(argv[3], &nbr))
	{
		// TODO: Should this print an error message?
		return (false);
	}
	data->time_to_eat = (t_time)nbr;
	if (!ph_atoi_safe(argv[4], &nbr))
	{
		// TODO: Should this print an error message?
		return (false);
	}
	data->time_to_sleep = (t_time)nbr;

	data->times_to_eat = 0;
	if (argc == 6)
	{
		if (!ph_atoi_safe(argv[5], &nbr))
		{
			// TODO: Should this print an error message?
			return (false);
		}
		data->times_to_eat = (size_t)nbr;
	}
	return (true);
}

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
	data->forks = NULL;
	data->philosophers = NULL;

	if (argc < 5 || argc > 6)
		return (print_error(PH_ERROR_WRONG_ARGUMENT_COUNT));

	if (!init_data_mutexes(data))
		return (print_error(PH_ERROR_INIT_MUTEX));

	init_argv(argc, argv, data);

	if (!init_forks(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	data->philosophers_eating = data->philosopher_count;
	data->running_program = true;
	data->running_philosophers = false;

	if (!create_philosophers(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	data->start_time = get_time();
	init_philosophers_time_of_last_meal(data);

	mutex_lock(&data->running_philosophers_mutex);
	data->running_philosophers = true;
	mutex_unlock(&data->running_philosophers_mutex);

	return (true);
}
