/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_private_atoi_safe.h                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 16:46:29 by sbos          #+#    #+#                 */
/*   Updated: 2022/10/25 16:46:29 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_PRIVATE_ATOI_SAFE_H
# define PH_PRIVATE_ATOI_SAFE_H

# include <limits.h>

# define WHITESPACE " \t\n\v\f\r"

t_i32	ph_atoi_range(const char *str, bool *out_of_range);

void	*ph_memchr(const void *ptr, t_i32 chr, size_t len);
size_t	ph_strlen(const char *str);
char	*ph_strchr(const char *str, t_i32 chr);
bool	ph_isspace(const t_i32 chr);
t_i32	ph_isdigit(t_i32 chr);

#endif
