/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:18:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/15 12:25:29 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Check if exit status is valid (between 0 and 255)
 *
 * Arguments:
 * - arg - status
 * 
 * Returns:
 * Status
 */
int	check_status(char *arg)
{
	int	status;
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i++]))
			return (status = -1);
	}
	status = ft_atoi(arg);
	if (status < 0 || status > 255)
		status = -1;
	return (status);
}

/*
 * Replicate exit command
 *
 * Arguments:
 * - args - cmd name and status
 * - info - info structure
 * 
 * Returns:
 * Status
 */
int	handle_exit(t_list *args, t_info *minishell)
{
	int	status;

	status = SUCCESS;
	if (args->next && args->next->next)
		return (print_error("exit: too many args\n", 1));
	if (args->next)
		status = check_status((char *)(args->next)->content);
	if (status == -1)
		return (print_error("exit status error\n", 1));
	minishell->exit_flag = 1;
	return (status);
}
