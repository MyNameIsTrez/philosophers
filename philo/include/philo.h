/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:12:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 13:01:13 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define LOOP_USLEEP 1000 // TODO: Better value

# define PTHREAD_SUCCESS 0

# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

# include "ph_typedefs.h"

# include "ph_init.h"
# include "ph_run.h"
# include "ph_utils.h"

#endif
