/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_size.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 12:41:20 by sbos          #+#    #+#                 */
/*   Updated: 2022/09/01 14:38:56 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "ph_parse_size.h"

bool	parse_size(const char *str, size_t *size_ptr)
{
	bool	out_of_range;
	int		i;
	int		range_result;

	i = 0;
	while (ph_isspace(str[i]))
		i++;
	if (str[i] == '-')
		i++;
	else if (str[i] == '+')
		i++;
	if (!ph_isdigit(str[i]))
		return (false);
	range_result = atoi_range(str, &out_of_range);
	if (out_of_range || range_result <= 0)
		return (false);
	*size_ptr = (size_t)range_result;
	while (ph_isdigit(str[i]))
		i++;
	while (ph_isspace(str[i]))
		i++;
	return (str[i] == '\0');
}
