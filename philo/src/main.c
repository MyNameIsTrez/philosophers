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
		// atexit(check_leaks); // TODO: REMOVE
		return (EXIT_FAILURE);
	}
	run(&data);
	mutex_lock(&data.running_program_mutex);
	data.running_program = false;
	mutex_unlock(&data.running_program_mutex);
	join_philosophers(data.philosopher_count, &data);
	destroy(&data);
	// atexit(check_leaks); // TODO: REMOVE
	return (EXIT_SUCCESS);
}
