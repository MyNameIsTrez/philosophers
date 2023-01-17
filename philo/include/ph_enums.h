/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_enums.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 15:16:30 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 15:16:30 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_ENUMS_H
# define PH_ENUMS_H

typedef enum e_event
{
	EVENT_FORK,
	EVENT_EAT,
	EVENT_SLEEP,
	EVENT_THINK,
	EVENT_DIED,
}	t_event;

typedef enum e_error
{
	ERROR_WRONG_ARGUMENT_COUNT,
	ERROR_INIT_MUTEX,
	ERROR_EXPECTED_NATURAL_NUMBER,
	ERROR_SYSTEM,
}	t_error;

#endif
