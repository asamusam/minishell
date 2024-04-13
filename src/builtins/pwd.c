/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:00 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/13 15:06:50 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Replicates pwd builtin command
 *
 * Arguments:
 * - t_list args
 *
 * Returns:
 * Status
 */
int	handle_pwd(t_info *info)
{
	if (info->pwd)
	{
		ft_printf("%s\n", info->pwd);//TODO:
		return (SUCCESS);
	}
	else
		ft_printf("%s\n", getenv("PWD"));
	return (SUCCESS);
}
