/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:57:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/09 15:26:45 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/*
 * Copies envp array received by main function to a list,
 * splitting its values into key and value pairs.
 * All the keys, values, and nodes are allocated with malloc().
 * 
 * Arguments:
 * - envp — envp array from main.
 * - minishell — general info structure.
 * 
 * Returns:
 * Nothing.
 */
void	create_envp_list(char **envp, t_info *minishell)
{
	int		i;
	char	**split;
	t_envp	*var;

	i = 0;
	minishell->envp_list = NULL;
	while (envp[i])
	{
		var = malloc(sizeof(t_envp));
		split = ft_split(envp[i], '=');
		if (!var || !split)
			exit(EXIT_FAILURE);
		var->key = ft_strdup(split[0]);
		if (split[1])
			var->value = ft_strdup(split[1]);
		else
			var->value = ft_strdup("");
		free_split(split);
		ft_lstadd_back(&minishell->envp_list, ft_lstnew(var));
		i ++;
	}
}

/*
 * Initializes the general info structure.
 * 
 * Arguments:
 * - envp — envp array from main.
 * - minishell — general info structure.
 * 
 * Returns:
 * Nothing.
 */
void	init(char **envp, t_info *minishell)
{
	create_info(minishell, envp);
	set_envp(minishell, envp);
	set_pwds(minishell);
	//create_envp_list(envp, minishell);
	//minishell->exit_code = 0;
}
