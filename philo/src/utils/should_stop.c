/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   should_stop.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/18 15:23:07 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/18 15:23:07 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	should_stop(t_data *data)
{
	mutex_lock(&data->running_program_mutex);
	if (!data->running_program)
	{
		mutex_unlock(&data->running_program_mutex);
		return (true);
	}
	mutex_unlock(&data->running_program_mutex);
	return (false);
}
