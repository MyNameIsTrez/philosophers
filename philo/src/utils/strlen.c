/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   strlen.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/17 16:02:46 by sbos          #+#    #+#                 */
/*   Updated: 2023/01/17 16:02:46 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ph_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}
