/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pthread.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 16:55:29 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 16:55:29 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_philosophers(size_t count, t_data *data)
{
	size_t			philosopher_index;
	t_philosopher	*philosopher;

	philosopher_index = 0;
	while (philosopher_index < count)
	{
		philosopher = &data->philosophers[philosopher_index];
		pthread_join(philosopher->thread, NULL);
		philosopher_index++;
	}
}
