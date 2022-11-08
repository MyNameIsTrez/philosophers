/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:15:55 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 12:47:50 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	mutex_lock(t_mutex *mutex_ptr)
{
	pthread_mutex_lock(&mutex_ptr->mutex);
}

static void	mutex_unlock(t_mutex *mutex_ptr)
{
	pthread_mutex_unlock(&mutex_ptr->mutex);
}

static void	mutex_destroy(t_mutex *mutex_ptr)
{
	if (mutex_ptr->initialized)
		pthread_mutex_destroy(&mutex_ptr->mutex);
}

static bool	mutex_init(t_mutex *mutex_ptr)
{
	if (pthread_mutex_init(&mutex_ptr->mutex, NULL) != 0)
		return (false);
	mutex_ptr->initialized = true;
	return (true);
}

// In milliseconds
static size_t	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: What if tv.tv_ms is -1? (signed microseconds after all)

	return ((size_t)tv.tv_sec * 1000 + (size_t)(tv.tv_usec / 1000));
}

static void	precise_sleep(t_philosopher *philosopher, size_t start_time, size_t duration)
{
	while (true)
	{
		mutex_lock(&philosopher->data->running_mutex);
		if (!philosopher->data->running || get_time() - start_time > duration)
		{
			mutex_unlock(&philosopher->data->running_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_mutex);

		usleep(LOOP_USLEEP);
	}
}

static void	print_event(t_event event, t_philosopher *philosopher)
{
	static bool	printing = true;
	static char	*event_strings[] = {
		[EVENT_FORK] = "has taken a fork",
		[EVENT_EAT] = "is eating",
		[EVENT_SLEEP] = "is sleeping",
		[EVENT_THINK] = "is thinking",
		[EVENT_DIED] = "died",
	};
	t_data		*data;

	data = philosopher->data;
	mutex_lock(&data->printf_mutex);
	// TODO: Do I need to do anything to make a morbillion% sure that get_time() - data->start_time won't ever underflow?
	if (printing)
	{
		printf("%lu %zu %s\n", get_time() - data->start_time, philosopher->index + 1, event_strings[event]);
		mutex_lock(&data->philosopher_count_eating_mutex);
		if (event == EVENT_DIED || (event == EVENT_EAT && data->philosopher_count_eating == 0))
			printing = false;
		mutex_unlock(&data->philosopher_count_eating_mutex);
	}
	mutex_unlock(&data->printf_mutex);
}

static void	run_single_philosopher_edgecase(t_philosopher *philosopher)
{
	mutex_lock(&philosopher->data->running_mutex);
	if (!philosopher->data->running) // TODO: Is this one necessary?
	{
		mutex_unlock(&philosopher->data->running_mutex);
		return ;
	}
	mutex_unlock(&philosopher->data->running_mutex);
	
	mutex_lock(philosopher->left_fork);
	
	print_event(EVENT_FORK, philosopher);

	while (true)
	{
		mutex_lock(&philosopher->data->running_mutex);
		if (!philosopher->data->running)
		{
			mutex_unlock(&philosopher->data->running_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_mutex);
		usleep(LOOP_USLEEP);
	}

	mutex_unlock(philosopher->left_fork);
}

static void	run_regular_philosopher(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 1)
		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat / 2);

	while (true)
	{
		mutex_lock(&philosopher->data->running_mutex);
		if (!philosopher->data->running) // TODO: Should this check be done after every action below?
		{
			mutex_unlock(&philosopher->data->running_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_mutex);

		if (philosopher->index % 2 == 0)
		{
			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher);

			mutex_lock(philosopher->right_fork);
			
			print_event(EVENT_FORK, philosopher);
		}
		else
		{
			mutex_lock(philosopher->right_fork);

			print_event(EVENT_FORK, philosopher);

			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher);
		}

		// TODO: This probably shouldn't happen when the philosopher hasn't eaten but is just quitting because another phil died.
		philosopher->times_eaten++;
		if (philosopher->times_eaten == philosopher->data->times_to_eat)
		{
			mutex_lock(&philosopher->data->philosopher_count_eating_mutex);
			philosopher->data->philosopher_count_eating--;
			mutex_unlock(&philosopher->data->philosopher_count_eating_mutex);
		}

		print_event(EVENT_EAT, philosopher);

		mutex_lock(&philosopher->time_of_last_meal_mutex);
		philosopher->time_of_last_meal = get_time();
		mutex_unlock(&philosopher->time_of_last_meal_mutex);

		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat);

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		mutex_unlock(philosopher->left_fork);
		mutex_unlock(philosopher->right_fork);

		print_event(EVENT_SLEEP, philosopher);

		size_t	time_of_last_sleep;

		time_of_last_sleep = get_time();

		// mutex_lock(&philosopher->data->printf_mutex);
		// printf("time to sleep: %zu\n", philosopher->data->time_to_sleep);
		// mutex_unlock(&philosopher->data->printf_mutex);

		precise_sleep(philosopher, time_of_last_sleep, philosopher->data->time_to_sleep);

		// mutex_lock(&philosopher->data->printf_mutex);
		// printf("%zu after sleeping\n", philosopher->index + 1);
		// mutex_unlock(&philosopher->data->printf_mutex);

		print_event(EVENT_THINK, philosopher);

		if (philosopher->data->philosopher_count % 2 == 1)
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
		mutex_lock(&philosopher->data->running_philosophers_mutex);
		running_philosophers = philosopher->data->running_philosophers;
		mutex_unlock(&philosopher->data->running_philosophers_mutex);

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

		philosopher->time_of_last_meal_mutex.initialized = false;
		if (!mutex_init(&philosopher->time_of_last_meal_mutex))
			return (false);

		// TODO: pthread_join() the previous philosophers when there's an error!
		if (pthread_create(&philosopher->thread, NULL, run_philosopher, philosopher) != 0)
			return (false);

		philosopher_index++;
	}

	return (true);
}

static bool	init_forks(t_data *data)
{
	size_t	fork_index;

	fork_index = 0;
	data->forks = malloc(data->philosopher_count * sizeof(pthread_mutex_t));
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

static bool	philosopher_starved(t_philosopher *philosopher)
{
	size_t	time_of_last_meal;
	t_data	*data;

	data = philosopher->data;

	// TODO: This is a race condition since time_of_last_meal may change during this function
	mutex_lock(&philosopher->time_of_last_meal_mutex);
	time_of_last_meal = philosopher->time_of_last_meal;
	mutex_unlock(&philosopher->time_of_last_meal_mutex);

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
			print_event(EVENT_DIED, philosopher);
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
		if (any_philosopher_starved(data))
			return ;
		mutex_lock(&data->philosopher_count_eating_mutex);
		if (data->philosopher_count_eating == 0)
		{
			mutex_unlock(&data->philosopher_count_eating_mutex);
			return ;
		}
		mutex_unlock(&data->philosopher_count_eating_mutex);
		usleep(LOOP_USLEEP);
	}
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

static bool	init_data_mutexes(t_data *data)
{
	data->running_mutex.initialized = false;
	data->printf_mutex.initialized = false;
	data->philosopher_count_eating_mutex.initialized = false;
	data->running_philosophers_mutex.initialized = false;
	return (
		mutex_init(&data->running_mutex) &&
		mutex_init(&data->printf_mutex) &&
		mutex_init(&data->philosopher_count_eating_mutex) &&
		mutex_init(&data->running_philosophers_mutex));
}

static bool	init_argv(int argc, char *argv[], t_data *data)
{
	int	nbr;
	
	// TODO: Throw an error in case an arg is < 0 or <= 0?

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
		data->times_to_eat = 0;
	return (true);
}

static bool	init(int argc, char *argv[], t_data *data)
{
	if (argc < 5 || argc > 6)
		return (write_error(PH_ERROR_WRONG_ARGUMENT_COUNT));

	if (!init_data_mutexes(data))
		return (write_error(PH_ERROR_INIT_MUTEX));

	init_argv(argc, argv, data);

	data->forks = NULL;
	data->philosophers = NULL;

	if (!init_forks(data))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (false);
	}
	
	data->philosopher_count_eating = data->philosopher_count;
	data->running = true;
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

static void	destroy_and_free(t_data *data)
{
	mutex_destroy(&data->running_mutex);
	mutex_destroy(&data->printf_mutex);
	mutex_destroy(&data->philosopher_count_eating_mutex);
	mutex_destroy(&data->running_philosophers_mutex);
	
	destroy_forks(data->philosopher_count, data);
	free(data->forks);
	
	destroy_philosophers(data->philosopher_count, data);
	free(data->philosophers);
}

// void	check_leaks(void)
// {
// 	system("leaks -q philo");
// }

int	main(int argc, char *argv[])
{
	t_data	data;

	if (!init(argc, argv, &data))
	{
		destroy_and_free(&data);
		// atexit(check_leaks); // TODO: Remove for eval
		return (EXIT_FAILURE);
	}

	run(&data); // TODO: Wrap in error check if-statement?

	mutex_lock(&data.running_mutex);
	data.running = false;
	mutex_unlock(&data.running_mutex);

	join_philosophers(&data); // TODO: Wrap in error check if-statement?

	destroy_and_free(&data);

	// atexit(check_leaks); // TODO: Remove for eval

	return (EXIT_SUCCESS);
}
