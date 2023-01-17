/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_error.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/17 15:13:35 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/17 15:13:35 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	print_error(t_error ph_error)
{
	char*	error_messages[] = {
		[PH_ERROR_WRONG_ARGUMENT_COUNT] = "Wrong argument count",
        [PH_ERROR_INIT_MUTEX] = "A mutex init failed",
		[PH_ERROR_EXPECTED_NATURAL_NUMBER] = "Expected a natural number",
	};
    char	*error_message;

    error_message = error_messages[ph_error];
	write(STDERR_FILENO, "Error: ", 7);
	write(STDERR_FILENO, error_message, ph_strlen(error_message));
	write(STDERR_FILENO, ".\n", 2);
	return (false);
}
