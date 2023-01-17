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
	PH_ERROR_WRONG_ARGUMENT_COUNT,
	PH_ERROR_INIT_MUTEX,
	PH_ERROR_EXPECTED_NATURAL_NUMBER,
}	t_error;

#endif
