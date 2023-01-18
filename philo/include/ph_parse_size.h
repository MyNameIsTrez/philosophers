/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_parse_size.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 16:46:29 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/25 16:46:29 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_PARSE_SIZE_H
# define PH_PARSE_SIZE_H

# include "philo.h"

# include <limits.h>

# define WHITESPACE " \t\n\v\f\r"

// init/parse_size/atoi_range.c
int		atoi_range(const char *str, bool *out_of_range);

// init/parse_size/utils.c
bool	ph_isspace(const int chr);
int		ph_isdigit(int chr);

#endif
