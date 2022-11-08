/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   atoi_range.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 17:01:27 by sbos          #+#    #+#                 */
/*   Updated: 2022/07/14 15:02:07 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "ph_private_atoi_safe.h"

static int	char_to_digit(const char chr)
{
	if ('0' <= chr && chr <= '9')
		return (chr - '0');
	return (-1);
}

/**
 * @brief Takes a @p str like "123" and returns -123. It returns a negative
 * number to circumvent overflow.
 *
 * @param str
 * @param out_of_range Set to true when multiplying by ten
 * or subtracting a digit results in underflow.
 * @return
 */
static int	get_negative_value(const char *str, bool *out_of_range)
{
	static const int	range_mult_ten = INT_MIN / 10;
	static const int	range_last_digit = -(INT_MIN % 10);
	int					value;
	size_t				i;
	int					digit;

	value = 0;
	i = 0;
	while (ph_isdigit(str[i]))
	{
		digit = char_to_digit(str[i]);
		if (value < range_mult_ten)
			*out_of_range = true;
		else if (value == range_mult_ten && digit > range_last_digit)
			*out_of_range = true;
		value *= 10;
		value -= digit;
		i++;
	}
	return (value);
}

/**
 * @brief Converts @p str to a int. This function is only supposed to be
 * used by atoi() and atoi_safe().
 *
 * @param str May start with whitespace, and the first encountered number will
 * be converted to a int.
 * @param out_of_range
 * @return The converted value or 0 if no number was found in @p str.
 */
int	ph_atoi_range(const char *str, bool *out_of_range)
{
	int	sign;
	int	value;
	bool	dummy_out_of_range;

	if (out_of_range == NULL)
		out_of_range = &dummy_out_of_range;
	*out_of_range = false;
	while (ph_isspace(str[0]))
		str++;
	sign = 1;
	if (str[0] == '-')
	{
		sign = -1;
		str++;
	}
	else if (str[0] == '+')
		str++;
	value = get_negative_value(str, out_of_range);
	if (sign == 1 && value == INT_MIN)
		*out_of_range = true;
	return (-sign * value);
}
