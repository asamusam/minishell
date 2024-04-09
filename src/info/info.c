/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:53:47 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/09 15:25:33 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	set_pwds(t_info *info)
{
	t_list	*enpv_list;
	char	*key;
	char	*paths;

	enpv_list = info->envp_list;
	while (enpv_list)
	{
		key = ((t_envp *)enpv_list->content)->key;
		if (key && !ft_strcmp(key, "PWD"))
			info->pwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "OLDPWD"))
			info->oldpwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "HOME"))
			info->home = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "PATH"))
		{
			paths = ft_strdup(((t_envp *)enpv_list->content)->value);
			if (info->path)
				free_split(info->path);
			info->path = ft_split(paths, ':');
			free(paths);
		}
		enpv_list = enpv_list->next;
	}
}

int	update_envstr(t_info *info)
{
	int		i;
	int		len;
	char	*tmp;
	t_list	*curr;

	i = 0;
	curr = info->envp_list;
	free_split(info->envp);
	len = ft_lstsize(info->envp_list);
	info->envp = malloc(sizeof(char *) * (len + 1));
	if (!info->envp)
		return (print_error("malloc error\n", 0));
	i = 0;
	while (curr)
	{
		tmp = ft_strjoin(((t_envp *)curr->content)->key, "=");
		if (((t_envp *)curr->content)->value)
			info->envp[i] = ft_strjoin(tmp, ((t_envp *)curr->content)->value);
		else
			info->envp[i] = ft_strdup(tmp);
		free(tmp);
		i++;
		curr = curr->next;
	}
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

void	set_envp(t_info *info, char **envp)
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
		if (!info->envp_list)
			info->envp_list = create_envp_node(key, value);
		else
			check_envs(info, key, value);
		free_split(keyval);
		free(key);
		free(value);
		current++;
	}
}

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
	info->is_multiple_proc = 0;
	return (info);
}
