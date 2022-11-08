/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_error.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/22 16:15:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 15:37:55 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_ERROR_H
# define PH_ERROR_H

typedef enum e_ph_error
{
	PH_ERROR_WRONG_ARGUMENT_COUNT,
	PH_ERROR_INIT_MUTEX,
}	t_ph_error;

bool	write_error(t_ph_error ph_error);

#endif

