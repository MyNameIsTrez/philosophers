/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 16:40:38 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/25 16:40:38 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "ph_parse_size.h"

static void	*ph_memchr(const void *ptr, int chr, size_t len)
{
	unsigned char	*ptr_;
	unsigned char	chr_;
	size_t			i;

	ptr_ = (unsigned char *)ptr;
	chr_ = (unsigned char)chr;
	i = 0;
	while (i < len)
	{
		if (ptr_[i] == chr_)
			return (&ptr_[i]);
		i++;
	}
	return (NULL);
}

static char	*ph_strchr(const char *str, int chr)
{
	return (ph_memchr(str, chr, ph_strlen(str) + 1));
}

bool	ph_isspace(const int chr)
{
	if (chr != '\0' && ph_strchr(WHITESPACE, chr))
		return (true);
	return (false);
}

int	ph_isdigit(int chr)
{
	return ('0' <= chr && chr <= '9');
}
