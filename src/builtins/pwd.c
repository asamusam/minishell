/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:00 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 14:25:38 by mmughedd         ###   ########.fr       */
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
int	handle_pwd(t_info *minishell)
{
	if (minishell->pwd)
		ft_printf("%s\n", minishell->pwd);
	else
		return (FAIL);
	return (SUCCESS);
}
