/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:53:47 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 12:18:06 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	free_mini_path(t_info *minishell)
{
	if (minishell->path)
	{
		free_split(minishell->path);
		minishell->path = NULL;
	}
}

void	set_pwds(t_info *minishell)
{
	t_list	*enpv_list;
	char	*key;
	char	*paths;

	enpv_list = minishell->envp_list;
	free_mini_path(minishell);
	while (enpv_list)
	{
		key = ((t_envp *)enpv_list->content)->key;
		if (key && !ft_strcmp(key, "PWD"))
			minishell->pwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "OLDPWD"))
			minishell->oldpwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "HOME"))
			minishell->home = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "PATH"))
		{
			paths = ft_strdup(((t_envp *)enpv_list->content)->value);
			minishell->path = ft_split(paths, ':');
			free(paths);
		}
		enpv_list = enpv_list->next;
	}
}

void	update_str(t_list *envp_list, t_info *mini, int i)
{
	char	*tmp;
	t_envp	*keyval;

	keyval = (t_envp *)envp_list->content;
	tmp = ft_strjoin(keyval->key, "=");
	if (keyval->value)
		mini->envp[i] = ft_strjoin(tmp, keyval->value);
	else
		mini->envp[i] = ft_strdup(tmp);
	free(tmp);
}

int	update_envstr(t_info *minishell)
{
	int		i;
	int		len;
	t_list	*curr;

	curr = minishell->envp_list;
	free_split(minishell->envp);
	len = ft_lstsize(minishell->envp_list);
	minishell->envp = malloc(sizeof(char *) * (len + 1));
	if (!minishell->envp)
		return (print_error(MALLOC_ERROR, PERROR));
	i = 0;
	while (curr)
	{
		update_str(curr, minishell, i);
		i++;
		curr = curr->next;
	}
	minishell->envp[i] = NULL;
	return (0);
}

void	set_envp(t_info *minishell, char **envp)
{
	char	**keyval;
	char	*key;
	char	*value;
	char	**current;

	current = envp;
	while (*current)
	{
		keyval = ft_split(*current, '=');
		key = ft_strdup(keyval[0]);
		if (keyval[1])
			value = ft_strdup(keyval[1]);
		else
			value = NULL;
		if (!minishell->envp_list)
			minishell->envp_list = create_envp_node(key, value);
		else
			check_envs(minishell, key, value);
		free_split(keyval);
		free(key);
		free(value);
		current++;
	}
}
