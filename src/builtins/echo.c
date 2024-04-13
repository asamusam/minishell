/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:35 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/12 09:52:31 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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
		ft_printf("%s", (char *)current->content);//ft_putstr_fd((char *)current->content, 1);
		current = current->next;
	}
	if (!is_n)
		ft_printf("\n", 1);
	return (0);
}
