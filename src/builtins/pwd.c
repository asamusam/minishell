/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:00 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/10 13:16:26 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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
		ft_printf("%s\n", info->pwd);
		return (SUCCESS);
	}
	else
		ft_printf("%s\n", getenv("PWD"));
	return (FAIL);
}
