/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mutex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 13:08:17 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 13:08:25 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_lock(t_mutex *mutex_ptr)
{
	pthread_mutex_lock(&mutex_ptr->mutex);
}

void	mutex_unlock(t_mutex *mutex_ptr)
{
	pthread_mutex_unlock(&mutex_ptr->mutex);
}

void	mutex_destroy(t_mutex *mutex_ptr)
{
	if (mutex_ptr->initialized)
		pthread_mutex_destroy(&mutex_ptr->mutex);
}

bool	mutex_init(t_mutex *mutex_ptr)
{
	if (pthread_mutex_init(&mutex_ptr->mutex, NULL) != PTHREAD_SUCCESS)
		return (false);
	mutex_ptr->initialized = true;
	return (true);
}
