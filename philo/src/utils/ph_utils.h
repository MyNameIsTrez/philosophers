/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_utils.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/22 16:15:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 15:37:55 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_UTILS_H
# define PH_UTILS_H

# include "ph_typedefs.h"

typedef enum e_ph_error
{
	PH_ERROR_WRONG_ARGUMENT_COUNT,
	PH_ERROR_INIT_MUTEX,
}	t_ph_error;

void	destroy(t_data *data);

void	mutex_lock(t_mutex *mutex_ptr);
void	mutex_unlock(t_mutex *mutex_ptr);
void	mutex_destroy(t_mutex *mutex_ptr);
bool	mutex_init(t_mutex *mutex_ptr);

bool	print_error(t_ph_error ph_error);
void	print_event(t_event event, t_philosopher *philosopher);

t_time	get_time(void);
void	precise_sleep(t_philosopher *philosopher, t_time start_time,
			t_time duration);

#endif
