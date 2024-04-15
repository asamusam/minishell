/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:53:47 by mmughedd          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/15 09:42:27 by mmughedd         ###   ########.fr       */
=======
/*   Updated: 2024/04/10 15:47:33 by asamuilk         ###   ########.fr       */
>>>>>>> free
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	set_pwds(t_info *minishell)
{
	t_list	*enpv_list;
	char	*key;
	char	*paths;

	enpv_list = minishell->envp_list;
	if (minishell->path)
	{
		free_split(minishell->path);
		minishell->path = NULL;
	}
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

int	update_envstr(t_info *minishell)
{
	int		i;
	int		len;
	char	*tmp;
	t_list	*curr;

	i = 0;
	curr = minishell->envp_list;
	free_split(minishell->envp);
	len = ft_lstsize(minishell->envp_list);
	minishell->envp = malloc(sizeof(char *) * (len + 1));
	if (!minishell->envp)
		return (print_error("malloc error\n", 0));
	i = 0;
	while (curr)
	{
		tmp = ft_strjoin(((t_envp *)curr->content)->key, "=");
		if (((t_envp *)curr->content)->value)
			minishell->envp[i] = ft_strjoin(tmp, ((t_envp *)curr->content)->value);
		else
			minishell->envp[i] = ft_strdup(tmp);
		free(tmp);
		i++;
		curr = curr->next;
	}
	minishell->envp[i] = NULL;
	return (0);
}

char	**copy_envp(char **envp)
{
	int		i;
	char	**envp_copy;

	if (!*envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	envp_copy = malloc(sizeof(char *) * (i + 1));
	if (!envp_copy)
	{
		print_error("malloc error\n", 0);
		return (NULL);
	}
	envp_copy[i] = NULL;
	i = -1;
	while (envp[++i])
		envp_copy[i] = ft_strdup(envp[i]);
	return (envp_copy);
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

<<<<<<< HEAD
=======
t_info	*create_info(t_info *info, char **envp)
{
	// t_info	*info;

	// info = malloc(sizeof(t_info));
	// if (!info)
	// {
	// 	print_error("Malloc error\n", 0);
	// 	return (NULL);
	// }
	info->envp = copy_envp(envp);
	info->pwd = NULL;
	info->oldpwd = NULL;
	info->home = NULL;
	info->path = NULL;
	info->envp_list = NULL;
	info->exit_code = 0;
	info->exit_flag = 0;
	info->is_multiple_proc = 0;
	return (info);
}
>>>>>>> free
