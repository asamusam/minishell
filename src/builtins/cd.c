/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/01 14:07:11 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	update_envp_pwd(t_info *info, char *newpwd)
{
	char	*oldpwd;
	char	*key;
	t_list	*envp_list;

	envp_list = info->envp_list;
	oldpwd = ft_strdup(info->pwd);
	while (envp_list)
	{
		key = ((t_envp *)envp_list->content)->key;
		if (key && !ft_strncmp(key, "PWD", 3))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = ft_strdup(newpwd);
		}
		else if (key && !ft_strncmp(key, "OLDPWD", 6))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = oldpwd;
		}
		envp_list = envp_list->next;
	}
	return (0);
}

int	dir_home(t_info *info)
{
	char	*dir;
	
	dir = getenv("HOME"); //  TODO: which home shall i use?
	if (dir == NULL) // 1. If no directory operand is given and the HOME environment variable is empty or undefined, the default behavior is implementation-defined and no further steps shall be taken.
	{
		print_error("HOME environment variable not set.\n", 0);
		return (1);
	}
	if (chdir(dir) == -1)
	{
		print_error("0 - chdir error\n", 0);
		return (1);
	}
	update_envp_pwd(info, dir);
	return (0);
}

int	dir_abs_path(t_info *info, char *dir)
{
	if (chdir(dir) == -1)
	{
		print_error("0 - chdir error\n", 0);
		return (1);
	}
	update_envp_pwd(info, dir);
	return(0);
}

int	dir_rel_path(t_info *info, char *dir)
{
	char	*dirpath;
	char	*cdpath;
	char	*tmp;
	
	cdpath = info->pwd;
	if (!cdpath)
	{
		print_error("1 - path not available\n", 0);
		return(1);
	}
	tmp = ft_strjoin(cdpath, "/");
	dirpath = ft_strjoin(tmp, dir);
	free (tmp);
	if (chdir(dirpath) == -1)
	{
		free(dirpath);
		print_error("2 - chdir error\n", 0);
		return (1);
	}
	update_envp_pwd(info, dirpath);
	free(dirpath);
	return(0);
}

int	handle_cd(t_list *args, t_info *info)
{
	char	*dir;
	char	**cdpaths;
	t_list *current;

	if (args->next && args->next->next)
	{
		print_error("bash: cd: too many arguments\n", 0);
		return(1);
	}
	else if (!args->next) // no directory provided
		return(dir_home(info));
	else
	{
		current = args->next;
		dir = (char *)current->content;
		if (!ft_strncmp(dir, "/", 1)) // absolute path
			return (dir_abs_path(info, dir));
		else
			return(dir_rel_path(info, dir));
	}
	return (0);
}