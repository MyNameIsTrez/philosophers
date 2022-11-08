/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   write_error.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 14:55:31 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 15:38:23 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static size_t   ph_strlen(char *str)
{
    size_t  i;
    
    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

bool	write_error(t_ph_error ph_error)
{
	char*	error_messages[] = {
		[PH_ERROR_WRONG_ARGUMENT_COUNT] = "Wrong argument count",
        [PH_ERROR_INIT_MUTEX] = "A mutex init failed",
	};
    char    *error_message;

    error_message = error_messages[ph_error];
	write(STDERR_FILENO, "Error: ", 7);
	write(STDERR_FILENO, error_message, ph_strlen(error_message));
	write(STDERR_FILENO, ".\n", 2);
	return (false);
}
