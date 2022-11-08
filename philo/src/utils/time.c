/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 15:31:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 15:31:18 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// In milliseconds
size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// TODO: What if tv.tv_ms is -1? (signed microseconds after all)

	return ((size_t)tv.tv_sec * 1000 + (size_t)(tv.tv_usec / 1000));
}

void	precise_sleep(t_philosopher *philosopher, size_t start_time, size_t duration)
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
