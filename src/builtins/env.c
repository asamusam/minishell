/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:41 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 11:49:25 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Replicates env builtin command
 *
 * Arguments:
 * - t_info info
 *
 * Returns:
 * Status
 */
int	handle_env(t_info *minishell)
{
	t_list	*current;
	t_envp	*envp;

	current = minishell->envp_list;
	while (current)
	{
		envp = (t_envp *)current->content;
		ft_printf("%s=%s\n", envp->key, envp->value);
		current = current->next;
	}
	return (SUCCESS);
}
