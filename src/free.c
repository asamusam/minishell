/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:02:42 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/10 15:42:30 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Frees all the memory associated with ft_split() return.
 * 
 * Arguments:
 * - arr — array of strings returned by ft_split()
 * 
 * Returns:
 * Nothing.
 */
void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
}

/*
 * Frees all the memory associated with the envp structure.
 * 
 * Arguments:
 * - arg — pointer to a structure of type t_envp.
 * 
 * Returns:
 * Nothing.
 */
void	free_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	free(var->key);
	free(var->value);
	free(var);
}

/*
 * Frees all the memory associated with the t_info structure.
 * 
 * Arguments:
 * - minishell — pointer to a structure of type t_info.
 * 
 * Returns:
 * Nothing.
 */
void	free_minishell_info(t_info *minishell)
{
	free_split(minishell->path);
	free_split(minishell->envp);
	ft_lstclear(&minishell->envp_list, free_envvar);
}
