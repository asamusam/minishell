/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:41 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/04 09:37:40 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
 * Replicates env builtin command
 *
 * Arguments:
 * - t_info info
 *
 * Returns:
 * Status
 */
int	handle_env(t_info *info)
{
	t_list	*current;
	t_envp	*envp;

	current = info->envp_list;
	while (current)
	{
		envp = (t_envp *)current->content;
		ft_printf("%s=%s\n", envp->key, envp->value);
		current = current->next;
	}
	return (0);
}
