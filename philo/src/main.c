/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:15:55 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/18 17:15:55 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_u64	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: What if tv.tv_ms is -1? (signed microseconds after all)

	return ((t_u64)tv.tv_sec * 1000 + (t_u64)(tv.tv_usec / 1000));
}

static void	print_event(size_t philosopher_index, t_event event)
{
	char	*event_strings[] = {
		[EVENT_FORK] = "has taken a fork",
		[EVENT_EAT] = "is eating",
		[EVENT_SLEEP] = "is sleeping",
		[EVENT_THINK] = "is thinking",
		[EVENT_DIED] = "died",
	};

	printf("%llu %zu %s\n", get_current_time(), philosopher_index, event_strings[event]);
}

static void	*run_philosopher(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = arg;

	while (true)
	{
		pthread_mutex_lock(&philosopher->data->running_philosophers_mutex); // TODO: Check for error?
		if (philosopher->data->running_philosophers)
		{
			pthread_mutex_unlock(&philosopher->data->running_philosophers_mutex); // TODO: Check for error?
			break;
		}
		pthread_mutex_unlock(&philosopher->data->running_philosophers_mutex); // TODO: Check for error?

		usleep(100); // TODO: Change to better value
	}

	while (philosopher->data->running) // TODO: Wrap this in a mutex?
	{
		if ((philosopher->index & 1) == 0)
		{
			if (pthread_mutex_lock(philosopher->left_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK);
			if (pthread_mutex_lock(philosopher->right_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK);
		}
		else
		{
			if (pthread_mutex_lock(philosopher->right_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK);
			if (pthread_mutex_lock(philosopher->left_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK);
		}

		philosopher->time_of_last_meal = get_current_time();

		usleep(1000000); // TODO: Change to better value

		// die if more than 10 ms passed

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		if (pthread_mutex_unlock(philosopher->left_fork) != 0)
		{
			// TODO: What to do in this case?
		}
		if (pthread_mutex_unlock(philosopher->right_fork) != 0)
		{
			// TODO: What to do in this case?
		}

		usleep(1000000); // TODO: Change to better value
	}

	return (NULL);
}

static bool	create_philosophers(t_data *data)
{
	size_t			philosopher_index;

	t_philosopher	*philosopher;

	data->philosophers = malloc(data->philosopher_count * sizeof(t_philosopher));
	if (data->philosophers == NULL)
		return (false);
	philosopher_index = 0;

	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];

		philosopher->index = philosopher_index;
		philosopher->time_of_last_meal = get_current_time();

		philosopher->left_fork = &data->forks[philosopher_index];
		philosopher->right_fork = &data->forks[(philosopher_index + 1) % data->philosopher_count];

		philosopher->data = data;

		if (pthread_create(&philosopher->thread, NULL, run_philosopher, philosopher) != 0) // TODO: Are default attributes OK?
		{
			// TODO: Free the previous philosophers when there's an error?
			return (false);
		}

		philosopher_index++;
	}

	return (true);
}

static bool	init_forks(t_data *data)
{
	size_t			fork_index;

	fork_index = 0;
	data->forks = malloc(data->philosopher_count * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return (false);
	while (fork_index < data->philosopher_count)
	{
		if (pthread_mutex_init(&data->forks[fork_index], NULL) != 0) // TODO: Are default attributes OK?
		{
			// TODO: Should this also be destroying the previously init mutexes?
			return (false);
		}
		fork_index++;
	}
	return (true);
}

static void	run(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	while (true)
	{
		philosopher_index = 0;
		while (philosopher_index < data->philosopher_count)
		{
			philosopher = &data->philosophers[philosopher_index];
			if (get_current_time() - philosopher->time_of_last_meal > 2000)
			{
				print_event(philosopher_index, EVENT_DIED);

				data->running = false;

				return ;
			}

			philosopher_index++;
		}

		usleep(1000); // TODO: Change to better value
	}
}

static void	join_philosophers(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		if (pthread_join(philosopher->thread, NULL) != 0) // TODO: Is having value_ptr at NULL ever not desired?
		{
			// TODO: ???
		}
		philosopher_index++;
	}
}

static bool	init(int argc, char *argv[], t_data *data)
{
	// TODO: Check argc and argv
	(void)argc;
	(void)argv;

	// TODO: Is this necessary? It allows me to call free() on these carelessly
	data->philosophers = NULL;
	data->forks = NULL;

	data->running = true;

	t_i32	tentative_philosopher_count;
	if (!ph_atoi_safe(argv[1], &tentative_philosopher_count))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	data->philosopher_count = (size_t)tentative_philosopher_count;

	if (!init_forks(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	data->running_philosophers = false;

	if (pthread_mutex_init(&data->running_philosophers_mutex, NULL) != 0) // TODO: Are default attributes OK?
	{
		// TODO: Should this be doing anything else?
		return (false);
	}

	if (!create_philosophers(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	pthread_mutex_lock(&data->running_philosophers_mutex); // TODO: Check for error?
	data->running_philosophers = true;
	pthread_mutex_unlock(&data->running_philosophers_mutex); // TODO: Check for error?

	return (true);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (!init(argc, argv, &data))
		return (EXIT_FAILURE);

	run(&data); // TODO: Wrap in error check if-statement?

	join_philosophers(&data); // TODO: Wrap in error check if-statement?

	return (EXIT_SUCCESS);
}
