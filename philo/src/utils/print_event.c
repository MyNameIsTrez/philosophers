/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_event.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 14:55:31 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 15:38:23 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_event(t_event event, t_philosopher *philosopher)
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
		mutex_lock(&data->philosophers_eating_mutex);
		if (event == EVENT_DIED || (event == EVENT_EAT && data->philosophers_eating == 0))
			printing = false;
		mutex_unlock(&data->philosophers_eating_mutex);
	}
	mutex_unlock(&data->printf_mutex);
}
