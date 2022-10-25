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

t_f64	get_current_ms_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: What if tv.tv_ms is -1? (signed microseconds after all)

	return (tv.tv_sec * 1000 + ((t_f64)tv.tv_usec) / 1000);
}

static char	*get_event_string(t_event event)
{
	char	*event_strings[] = {
		[EVENT_FORK] = "has taken a fork",
		[EVENT_SLEEP] = "???some sleeping string???",
		[EVENT_EAT] = "is eating",
	};

	return (event_strings[event]);
}

static void	print_event(size_t philosopher_index, t_event event)
{
	printf("%.0f %zu %s\n", get_current_ms_time(), philosopher_index, get_event_string(event));
}

static void	*run_philosopher(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = arg;

	while (true)
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

		philosopher->ms_time_of_last_meal = get_current_ms_time();

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

static t_philosopher	*create_philosophers(size_t philosopher_count, pthread_mutex_t *forks)
{
	t_philosopher	*philosophers;
	size_t			philosopher_index;

	t_philosopher	*philosopher;

	philosophers = malloc(philosopher_count * sizeof(t_philosopher));
	if (philosophers == NULL)
		return (NULL);
	philosopher_index = 0;

	while (philosopher_index < philosopher_count)
	{
		philosopher = &philosophers[philosopher_index];

		philosopher->index = philosopher_index;
		philosopher->ms_time_of_last_meal = get_current_ms_time();

		philosopher->left_fork = &forks[philosopher_index];
		philosopher->right_fork = &forks[(philosopher_index + 1) % philosopher_count];

		if (pthread_create(&philosopher->thread, NULL, run_philosopher, philosopher) != 0) // TODO: Are default attributes OK?
		{
			// TODO: Free the previous philosophers when there's an error?
			return (NULL);
		}

		philosopher_index++;
	}

	return (philosophers);
}

pthread_mutex_t	*init_forks(size_t fork_count)
{
	size_t			fork_index;
	pthread_mutex_t	*forks;

	fork_index = 0;
	forks = malloc(fork_count * sizeof(pthread_mutex_t));
	while (fork_index < fork_count)
	{
		if (pthread_mutex_init(&forks[fork_index], NULL) != 0) // TODO: Are default attributes OK?
		{
			// TODO: Should this also be destroying the previously init mutexes?
			return (NULL);
		}
		fork_index++;
	}

	return (forks);
}

int	main(int argc, char *argv[])
{
	// TODO: Check argc and argv
	(void)argc;

	t_i32	tentative_philosopher_count;
	if (!ph_atoi_safe(argv[1], &tentative_philosopher_count))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (EXIT_FAILURE);
	}

	size_t	philosopher_count = (size_t)tentative_philosopher_count;

	pthread_mutex_t	*forks;
	forks = init_forks(philosopher_count);
	if (forks == NULL)
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (EXIT_FAILURE);
	}

	t_philosopher	*philosophers;
	philosophers = create_philosophers(philosopher_count, forks);
	if (philosophers == NULL)
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (EXIT_FAILURE);
	}

	while (true)
	{
		size_t	philosopher_index = 0;
		while (philosopher_index < philosopher_count)
		{
			if (get_current_ms_time() - philosophers[philosopher_index].ms_time_of_last_meal > 3e3)
			{
				printf("???philosopher %zu died???\n", philosopher_index);
				return (EXIT_FAILURE);
			}

			philosopher_index++;
		}

		usleep(1000); // TODO: Change to better value
	}

	return (EXIT_SUCCESS);
}
