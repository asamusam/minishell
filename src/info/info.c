/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:53:47 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/13 09:42:48 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	free_pwds(t_info *info)
{
	if (info->pwd)
		free(info->pwd);
	if (info->oldpwd)
		free(info->oldpwd);
	if (info->home)
		free(info->home);
	if (info->path)
		free_split(info->path);
	info->pwd = NULL;
	info->oldpwd = NULL;
	info->home = NULL;
	info->path = NULL;
}

void	set_pwds(t_info *info)
{
	t_list	*enpv_list;
	char	*key;
	char	*value;
	char	*paths;

	enpv_list = info->envp_list;
	free_pwds(info);
	while (enpv_list)
	{
		key = ((t_envp *)enpv_list->content)->key;
		value = ((t_envp *)enpv_list->content)->value;
		if (key && !ft_strcmp(key, "PWD"))
			info->pwd = ft_strdup(value); //((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "OLDPWD"))
			info->oldpwd = ft_strdup(value);//((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "HOME"))
			info->home = ft_strdup(value);//((t_envp *)enpv_list->content)->value;
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
			info->envp[i] = ft_strdup(tmp); //TODO: empty str
		free(tmp);
		i++;
		curr = curr->next;
	}
	info->envp[i] = NULL;
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
	info->envp = copy_envp(envp);
	info->pwd = NULL;
	info->oldpwd = NULL;
	info->home = NULL;
	info->path = NULL;
	info->envp_list = NULL;
	info->exit_code = 0;
	return (info);
}
