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

static void	*run_philosopher(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = arg;

	while (true)
	{
		// if ()
		// {
		// }

		// die if more than 10 ms passed

		usleep(1000000); // TODO: Change to better value
	}

	return (NULL);
}

static t_philosopher	*create_philosophers(size_t philosopher_count)
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

		philosopher->philosopher_index = philosopher_index;
		philosopher->ms_time_of_last_meal = get_current_ms_time();

		if (pthread_create(&philosopher->thread, NULL, run_philosopher, philosopher) != 0)
		{
			// TODO: Free the previous philosophers when there's an error?
			return (NULL);
		}

		philosopher_index++;
	}

	return (philosophers);
}

int	main(int argc, char *argv[])
{
	t_philosopher	*philosophers;

	// TODO: Check argc and argv

	t_i32	tentative_philosopher_count;

	(void)argc;

	if (!ph_atoi_safe(argv[1], &tentative_philosopher_count))
	{
		// TODO: Should this be returning EXIT_FAILURE?
		// TODO: And should it also print an error message?
		return (EXIT_FAILURE);
	}

	size_t	philosopher_count = (size_t)tentative_philosopher_count;

	philosophers = create_philosophers(philosopher_count);
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
				printf("foo\n");
				return (EXIT_FAILURE);
			}

			philosopher_index++;
		}

		usleep(1000); // TODO: Change to better value
	}

	return (EXIT_SUCCESS);
}
