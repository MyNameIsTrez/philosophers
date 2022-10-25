/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:12:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/18 17:12:18 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>

# include "types.h"

typedef struct s_philosopher
{
	size_t		philosopher_index;
	t_f64		ms_time_of_last_meal; // TODO: What type should I be using here?
	pthread_t	thread;
}	t_philosopher;

bool	ph_atoi_safe(const char *str, t_i32 *nbr);

#endif
