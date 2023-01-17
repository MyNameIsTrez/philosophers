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

t_time	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_time)tv.tv_sec * 1000 + (t_time)(tv.tv_usec / 1000));
}

void	precise_sleep(t_philosopher *philosopher, t_time start_time, t_time duration)
{
	while (true)
	{
		mutex_lock(&philosopher->data->running_program_mutex);
		if (!philosopher->data->running_program || get_time_ms() - start_time > duration)
		{
			mutex_unlock(&philosopher->data->running_program_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_program_mutex);

		usleep(LOOP_USLEEP);
	}
}
