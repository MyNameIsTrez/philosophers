/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   atoi_safe.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 12:41:20 by sbos          #+#    #+#                 */
/*   Updated: 2022/09/01 14:38:56 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "atoi_safe/ph_private_atoi_safe.h"

/**
 * @brief Converts @p str to a int and puts it in the pointer @p nbr.
 * This function is safer than atoi() because it returns a boolean that
 * is false when no number was found in @p str.
 * With atoi("0") and atoi("a") you can't tell whether @p str
 * contains a number, because those calls both return 0.
 * If the return value is false, the value of @p nbr is undefined.
 *
 * @param str May start with whitespace, and the first encountered number will
 * be converted to a int.
 * @param nbr The int will be put in here.
 * The dereferenced value won't be touched if no number was found in the string.
 * @return true if a number was found in @p str, false otherwise.
 */
bool	ph_atoi_safe(const char *str, int *nbr)
{
	bool	out_of_range;
	int	i;

	i = 0;
	while (ph_isspace(str[i]))
		i++;
	if (str[i] == '-')
		i++;
	else if (str[i] == '+')
		i++;
	if (!ph_isdigit(str[i]))
		return (false);
	*nbr = ph_atoi_range(str, &out_of_range);
	if (out_of_range)
		return (false);
	while (ph_isdigit(str[i]))
		i++;
	while (ph_isspace(str[i]))
		i++;
	return (str[i] == '\0');
}
