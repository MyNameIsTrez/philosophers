/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:15:55 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 13:46:45 by sbos          ########   odam.nl         */
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

static void	precise_sleep(t_philosopher *philosopher, size_t start_time, size_t duration)
{
	while (true)
	{
		pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
		if (!philosopher->data->running || get_time() - start_time > duration)
		{
			pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
			break ;
		}
		pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?

		usleep(LOOP_USLEEP);
	}
}

static void	print_event(size_t philosopher_index, t_event event, t_data *data)
{
	static bool	printing = true;
	char	*event_strings[] = {
		[EVENT_FORK] = "has taken a fork",
		[EVENT_EAT] = "is eating",
		[EVENT_SLEEP] = "is sleeping",
		[EVENT_THINK] = "is thinking",
		[EVENT_DIED] = "died",
	};

	pthread_mutex_lock(&data->printf_mutex);
	// TODO: Do I need to do anything to make a morbillion% sure that get_time() - data->start_time won't ever underflow?
	if (printing)
	{
		printf("%lu %zu %s\n", get_time() - data->start_time, philosopher_index + 1, event_strings[event]);
		pthread_mutex_lock(&data->philosophers_still_eating_mutex); // TODO: Check for error?
		if (event == EVENT_DIED || (event == EVENT_EAT && data->philosophers_still_eating == 0))
			printing = false;
		pthread_mutex_unlock(&data->philosophers_still_eating_mutex); // TODO: Check for error?
	}
	pthread_mutex_unlock(&data->printf_mutex);
}

static void	run_single_philosopher_edgecase(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
	if (!philosopher->data->running) // TODO: Is this one necessary?
	{
		pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
		return ;
	}
	pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
	
	if (pthread_mutex_lock(philosopher->left_fork) != 0)
	{
		// TODO: What to do in this case?
	}
	
	print_event(philosopher->index, EVENT_FORK, philosopher->data);

	while (true)
	{
		pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
		if (!philosopher->data->running)
		{
			pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
			break ;
		}
		pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
		usleep(LOOP_USLEEP);
	}

	if (pthread_mutex_unlock(philosopher->left_fork) != 0)
	{
		// TODO: What to do in this case?
	}
}

static void	run_regular_philosopher(t_philosopher *philosopher)
{
	if ((philosopher->index & 1) == 1)
		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat / 2);

	while (true)
	{
		pthread_mutex_lock(&philosopher->data->running_mutex); // TODO: Check for error?
		if (!philosopher->data->running) // TODO: Should this check be done after every action below?
		{
			pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?
			break ;
		}
		pthread_mutex_unlock(&philosopher->data->running_mutex); // TODO: Check for error?

		if ((philosopher->index & 1) == 0)
		{
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

		// TODO: This probably shouldn't happen when the philosopher hasn't eaten but is just quitting because another phil died.
		philosopher->times_eaten++;
		if (philosopher->times_eaten == philosopher->data->times_to_eat)
		{
			pthread_mutex_lock(&philosopher->data->philosophers_still_eating_mutex); // TODO: Check for error?
			philosopher->data->philosophers_still_eating--;
			pthread_mutex_unlock(&philosopher->data->philosophers_still_eating_mutex); // TODO: Check for error?
		}

		print_event(philosopher->index, EVENT_EAT, philosopher->data);

		pthread_mutex_lock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?
		philosopher->time_of_last_meal = get_time();
		pthread_mutex_unlock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?

		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat);

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		if (pthread_mutex_unlock(philosopher->left_fork) != 0)
		{
			// TODO: What to do in this case?
		}
		if (pthread_mutex_unlock(philosopher->right_fork) != 0)
		{
			// TODO: What to do in this case?
		}

		print_event(philosopher->index, EVENT_SLEEP, philosopher->data);

		size_t	time_of_last_sleep;

		time_of_last_sleep = get_time();

		// pthread_mutex_lock(&philosopher->data->printf_mutex);
		// printf("time to sleep: %zu\n", philosopher->data->time_to_sleep);
		// pthread_mutex_unlock(&philosopher->data->printf_mutex);

		precise_sleep(philosopher, time_of_last_sleep, philosopher->data->time_to_sleep);

		// pthread_mutex_lock(&philosopher->data->printf_mutex);
		// printf("%zu after sleeping\n", philosopher->index + 1);
		// pthread_mutex_unlock(&philosopher->data->printf_mutex);

		print_event(philosopher->index, EVENT_THINK, philosopher->data);

		if ((philosopher->index & 1) == 0)
			precise_sleep(philosopher, get_time(), philosopher->data->time_to_eat / 2);
	}
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
			break ;

		usleep(LOOP_USLEEP);
	}

	if (philosopher->data->philosopher_count == 1)
		run_single_philosopher_edgecase(philosopher);
	else
		run_regular_philosopher(philosopher);
	
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

		philosopher->times_eaten = 0;

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

static bool	philosopher_starved(t_philosopher *philosopher)
{
	size_t	time_of_last_meal;
	t_data	*data;

	data = philosopher->data;

	pthread_mutex_lock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?
	time_of_last_meal = philosopher->time_of_last_meal;
	pthread_mutex_unlock(&philosopher->time_of_last_meal_mutex); // TODO: Check for error?

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
			print_event(philosopher->index, EVENT_DIED, data);
			return (true);
		}

		philosopher_index++;
	}
	return (false);
}

static void	run(t_data *data)
{
	while (true)
	{
		pthread_mutex_lock(&data->philosophers_still_eating_mutex); // TODO: Check for error?
		if (any_philosopher_starved(data) || data->philosophers_still_eating == 0)
		{
			pthread_mutex_unlock(&data->philosophers_still_eating_mutex); // TODO: Check for error?
			return ;
		}
		pthread_mutex_unlock(&data->philosophers_still_eating_mutex); // TODO: Check for error?
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
	data->time_to_sleep = (size_t)nbr;

	// TODO: What to do if this argument isn't given?
	if (argc == 6)
	{
		if (!ph_atoi_safe(argv[5], &nbr))
		{
			// TODO: Should this be returning EXIT_FAILURE?
			// TODO: And should it also print an error message?
			return (false);
		}
		data->times_to_eat = (size_t)nbr;
	}
	else
	{
		data->times_to_eat = 0;
	}

	if (!init_forks(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}
	
	data->philosophers_still_eating = data->philosopher_count;
	if (pthread_mutex_init(&data->philosophers_still_eating_mutex, NULL) != 0) // TODO: Are default attributes OK?
	{
		// TODO: Free the previous philosophers when there's an error?
		// TODO: Should this also be destroying the previously init mutexes?
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

static void	destroy_fork_mutexes(t_data *data)
{
	size_t			fork_index;

	fork_index = 0;
	while (fork_index < data->philosopher_count)
	{
		pthread_mutex_destroy(&data->forks[fork_index]);
		fork_index++;
	}
}

static void	destroy_philosopher_mutexes(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		pthread_mutex_destroy(&philosopher->time_of_last_meal_mutex);
		philosopher_index++;
	}
}

static void	destroy_mutexes(t_data *data)
{
	destroy_fork_mutexes(data);
	destroy_philosopher_mutexes(data);
	pthread_mutex_destroy(&data->running_mutex);
	pthread_mutex_destroy(&data->printf_mutex);
	pthread_mutex_destroy(&data->philosophers_still_eating_mutex);
	pthread_mutex_destroy(&data->running_philosophers_mutex);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (!init(argc, argv, &data))
		return (EXIT_FAILURE);

	run(&data); // TODO: Wrap in error check if-statement?

	pthread_mutex_lock(&data.running_mutex); // TODO: Check for error?
	data.running = false;
	pthread_mutex_unlock(&data.running_mutex); // TODO: Check for error?

	join_philosophers(&data); // TODO: Wrap in error check if-statement?

	destroy_mutexes(&data);

	return (EXIT_SUCCESS);
}
