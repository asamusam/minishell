/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:35 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/15 05:56:48 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Replicates echo builtin command, checks for -n option
 *
 * Arguments:
 * args - argument to print
 *
 * Returns:
 * Status
 */
int	handle_echo(t_list *args)
{
	t_list	*current;
	bool	is_n;

	is_n = false;
	current = args;
	current = current->next;
	if (current && !ft_strcmp((char *)current->content, "-n"))
	{
		is_n = true;
		current = current->next;
	}
	while (current)
	{
		ft_putstr_fd((char *)current->content, STDOUT_FILENO);
		if (current->next)
			ft_putstr_fd(" ", STDOUT_FILENO);
		current = current->next;
	}
	if (!is_n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}
