/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/02 11:33:35 by mmughedd         ###   ########.fr       */
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
	
	dir = info->home;
	if (dir == NULL) // 1. If no directory operand is given and the HOME environment variable is empty or undefined, the default behavior is implementation-defined and no further steps shall be taken.
		return (print_error("bash: cd: HOME not set\n", 0));
	if (access(dir, F_OK) == -1)
		return (print_error("bash: cd: d: No such file or directory\n", 0));
	update_envp_pwd(info, dir);
	return (0);
}

int	dir_abs_path(t_info *info, char *dir)
{
	if (access(dir, F_OK) == -1)
		return (print_error("bash: cd: d: No such file or directory\n", 0));
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
		return (print_error("No path available\n", 0));
	tmp = ft_strjoin(cdpath, "/");
	dirpath = ft_strjoin(tmp, dir);
	free (tmp);
	if (access(dirpath, F_OK) == -1)
		return (print_error("bash: cd: d: No such file or directory\n", 0));
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
		return (print_error("bash: cd: too many arguments\n", 0));
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