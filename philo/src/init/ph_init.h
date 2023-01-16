/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ph_init.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/22 16:15:18 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/03 15:37:55 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_INIT_H
# define PH_INIT_H

# include "ph_typedefs.h"

bool	ph_str_to_size(const char *str, size_t *size_ptr);
bool	init(int argc, char *argv[], t_data *data);

#endif
