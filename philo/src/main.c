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

// In milliseconds
static size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: What if tv.tv_ms is -1? (signed microseconds after all)

	return ((size_t)tv.tv_sec * 1000 + (size_t)(tv.tv_usec / 1000));
}

static void	print_event(size_t philosopher_index, t_event event, t_data *data)
{
	static bool	running = true;
	char	*event_strings[] = {
		[EVENT_FORK] = "has taken a fork",
		[EVENT_EAT] = "is eating",
		[EVENT_SLEEP] = "is sleeping",
		[EVENT_THINK] = "is thinking",
		[EVENT_DIED] = "died",
	};

	pthread_mutex_lock(&data->printf_mutex);
	// TODO: Do I need to do anything to make a billion% sure that get_time() - data->start_time won't ever underflow?
	if (running)
		printf("%lu %zu %s\n", get_time() - data->start_time, philosopher_index + 1, event_strings[event]);
	if (event == EVENT_DIED)
		running = false;
	pthread_mutex_unlock(&data->printf_mutex);
}

static void	*run_philosopher(void *arg)
{
	t_philosopher	*philosopher;
	bool			running_philosophers;

	philosopher = arg;

	while (true)
	{
		pthread_mutex_lock(&philosopher->data->running_philosophers_mutex); // TODO: Check for error?
		running_philosophers = philosopher->data->running_philosophers;
		pthread_mutex_unlock(&philosopher->data->running_philosophers_mutex); // TODO: Check for error?

		if (running_philosophers)
			break;

		usleep(LOOP_USLEEP);
	}

	bool	running;

	while (true) // TODO: Wrap this in a mutex?
	{
		pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
		running = philosopher->data->running;
		pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?

		if (!running)
			break;

		if ((philosopher->index & 1) == 0)
		{
			// pthread_mutex_lock(&philosopher->data->printf_mutex);
			// printf("philosopher 1 wants to grab a fork\n");
			// pthread_mutex_unlock(&philosopher->data->printf_mutex);

			if (pthread_mutex_lock(philosopher->left_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK, philosopher->data);

			if (pthread_mutex_lock(philosopher->right_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK, philosopher->data);
		}
		else
		{
			// pthread_mutex_lock(&philosopher->data->printf_mutex);
			// printf("philosopher 2 wants to grab a fork\n");
			// pthread_mutex_unlock(&philosopher->data->printf_mutex);

			if (pthread_mutex_lock(philosopher->right_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK, philosopher->data);

			if (pthread_mutex_lock(philosopher->left_fork) != 0)
			{
				// TODO: What to do in this case?
			}
			print_event(philosopher->index, EVENT_FORK, philosopher->data);
		}

		print_event(philosopher->index, EVENT_EAT, philosopher->data);

		pthread_mutex_lock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?
		philosopher->time_of_last_meal = get_time();
		pthread_mutex_unlock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?

		while (true)
		{
			pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
			running = philosopher->data->running;
			pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?

			if (!running || get_time() - philosopher->time_of_last_meal > philosopher->data->time_to_eat)
				break;

			usleep(LOOP_USLEEP);
		}

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		if (pthread_mutex_unlock(philosopher->left_fork) != 0)
		{
			// TODO: What to do in this case?
		}
		if (pthread_mutex_unlock(philosopher->right_fork) != 0)
		{
			// TODO: What to do in this case?
		}

		// TODO: Do I need a usleep() here?
		// usleep(1000000);
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

		philosopher->left_fork = &data->forks[philosopher_index];
		philosopher->right_fork = &data->forks[(philosopher_index + 1) % data->philosopher_count];

		philosopher->data = data;

		if (pthread_mutex_init(&philosopher->time_of_last_meal_mutex, NULL) != 0) // TODO: Are default attributes OK?
		{
			// TODO: Free the previous philosophers when there's an error?
			// TODO: Should this also be destroying the previously init mutexes?
			return (false);
		}

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
	size_t			time_of_last_meal;

	while (true)
	{
		philosopher_index = 0;
		while (philosopher_index < data->philosopher_count)
		{
			philosopher = &data->philosophers[philosopher_index];

			pthread_mutex_lock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?
			time_of_last_meal = philosopher->time_of_last_meal;
			pthread_mutex_unlock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?

			if (get_time() - time_of_last_meal > data->time_to_die)
			{
				print_event(philosopher_index, EVENT_DIED, data);

				pthread_mutex_lock(&data->running_mutex); // TODO: Check for error?
				data->running = false;
				pthread_mutex_unlock(&data->running_mutex); // TODO: Check for error?

				return ;
			}

			philosopher_index++;
		}

		usleep(LOOP_USLEEP);
	}
}

static void	init_philosophers_time_of_last_meal(t_data *data)
{
	size_t			time;
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	time = get_time();
	data->start_time = time;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		philosopher->time_of_last_meal = time;
		philosopher_index++;
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
	if (pthread_mutex_init(&data->running_mutex, NULL) != 0) // TODO: Are default attributes OK?
	{
		// TODO: Should this be doing anything else?
		return (false);
	}

	if (pthread_mutex_init(&data->printf_mutex, NULL) != 0) // TODO: Are default attributes OK?
	{
		// TODO: Should this be doing anything else?
		return (false);
	}

	// TODO: Throw an error in case an arg is < 0 or <= 0?

	int	nbr;
	if (!ph_atoi_safe(argv[1], &nbr))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}
	data->philosopher_count = (size_t)nbr;
	if (!ph_atoi_safe(argv[2], &nbr))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}
	data->time_to_die = (size_t)nbr;
	if (!ph_atoi_safe(argv[3], &nbr))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}
	data->time_to_eat = (size_t)nbr;
	if (!ph_atoi_safe(argv[4], &nbr))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}

	// TODO: What to do if this argument isn't given?
	if (argc == 6)
	{
		data->time_to_sleep = (size_t)nbr;
		if (!ph_atoi_safe(argv[5], &nbr))
		{
			// TODO: Should this be returning EXIT_FAILURE?
			// TODO: And should it also print an error message?
			return (false);
		}
		data->number_of_times_each_philosopher_must_eat = (size_t)nbr;
	}
	else
	{
		data->number_of_times_each_philosopher_must_eat = 0;
	}

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

	init_philosophers_time_of_last_meal(data);

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
