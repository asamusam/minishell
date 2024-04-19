/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:57:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/19 09:02:35 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

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
	minishell->envp = copy_envp(envp);
	minishell->envp_list = NULL;
	minishell->pwd = NULL;
	minishell->oldpwd = NULL;
	minishell->home = NULL;
	minishell->path = NULL;
	minishell->exit_code = 0;
	minishell->exit_flag = 0;
	set_envp(minishell, envp);
	set_pwds(minishell);
}
