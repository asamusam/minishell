/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:41 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/28 11:27:59 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Replicates env builtin command
 *
 * Arguments:
 * - t_info *struct
 *
 * Returns:
 * Status
 */
int handle_env(t_info *info)
{
	int i;

	i = 0;
	while ((info->envp)[i])
		ft_putendl_fd((info->envp)[i++], 1);
	return (0);
}