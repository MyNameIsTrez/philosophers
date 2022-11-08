/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:15:55 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 13:21:31 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_philosophers(t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < data->philosopher_count)
	{
		philosopher = &data->philosophers[philosopher_index];
		if (pthread_join(philosopher->thread, NULL) != PTHREAD_SUCCESS) // TODO: Is having value_ptr at NULL ever not desired?
		{
			// TODO: ???
		}
		philosopher_index++;
	}
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
		destroy(&data);
		// atexit(check_leaks);
		return (EXIT_FAILURE);
	}

	run(&data); // TODO: Wrap in error check if-statement?

	mutex_lock(&data.running_mutex);
	data.running = false;
	mutex_unlock(&data.running_mutex);

	join_philosophers(&data); // TODO: Wrap in error check if-statement?

	destroy(&data);

	// atexit(check_leaks);

	return (EXIT_SUCCESS);
}
