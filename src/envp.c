/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:57:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 21:05:26 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	printf("----------\nkey: %s\nvalue: %s\npair: %s\n", \
			var->key, var->value, *var->pair);
}

void	free_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	free(var->key);
	free(var->value);
	free(var);
}

void	copy_envp(char **envp, t_info *minishell)
{
	int	i;

	i = 0;
	while (envp[i])
		i ++;
	minishell->envp = malloc((i + 1) * sizeof(char *));
	if (!minishell->envp)
		exit(EXIT_FAILURE);
	i = 0;
	while (envp[i])
	{
		minishell->envp[i] = ft_strdup(envp[i]);
		i ++;
	}
	minishell->envp[i] = NULL;
}

void	create_envp_list(t_info *minishell)
{
	int		i;
	char	**split;
	t_envp	*var;

	i = 0;
	minishell->envp_list = NULL;
	while (minishell->envp[i])
	{
		var = malloc(sizeof(t_envp));

		split = ft_split(minishell->envp[i], '=');
		if (!var || !split)
			exit(EXIT_FAILURE);
		var->key = ft_strdup(split[0]);
		if (split[1])
			var->value = ft_strdup(split[1]);
		else
			var->value = ft_strdup("");
		free_split(split);
		var->pair = &minishell->envp[i];
		ft_lstadd_back(&minishell->envp_list, ft_lstnew(var));
		i ++;
	}
}

void	init_envp(char **envp, t_info *minishell)
{
	copy_envp(envp, minishell);
	create_envp_list(minishell);
	minishell->envp_flag = 0;
}
