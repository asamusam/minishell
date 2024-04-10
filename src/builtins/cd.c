/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/10 11:55:23 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
 * Updates PWD and OLDPWD env variables
 *
 * Arguments:
 * - info - info structure
 * - newpwd - newpwd name variable 
 * cd
 * Returns:
 * 0 on success, 1 on error
 */
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
			if (info->pwd)
			{
				free(info->pwd);
				info->pwd = ft_strdup(newpwd);
			}
		}
		else if (key && !ft_strncmp(key, "OLDPWD", 6))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = oldpwd;
			if (info->oldpwd)
			{
				free(info->oldpwd);
				info->oldpwd = ft_strdup(oldpwd);
			}
		}
		envp_list = envp_list->next;
	}
	update_envstr(info);
	return (0);
}

/*
 * Looks for home directory if valid it updates the enp pwd/oldpwd
 *
 * Arguments:
 * - info - info structure
 * 
 * Returns:
 * 0 on success, 1 on error
 */
int	dir_home(t_info *info)
{
	char	*dir;

	dir = info->home;
	if (dir == NULL)
		return (print_error("bash: cd: HOME not set\n", 0));
	if (access(dir, F_OK) == -1)
		return (print_error("bash: cd: d: No such file or directory\n", 0));
	update_envp_pwd(info, dir);
	chdir(dir);
	return (0);
}

/*
 * Checks if the absolute path is valid and if so updates the envp pwd/oldpwd
 *
 * Arguments:
 * - dir - aboslute path
 * - info - info structure
 * 
 * Returns:
 * 0 on success, 1 on error
 */
int	dir_abs_path(t_info *info, char *dir)
{
	char	*new_dir;
	
	if (dir[0] == '~')
	{
		if (!info->home)
			return(print_error("no home defined\n", 1));
		dir++;
		new_dir = ft_strjoin(info->home, dir);
	}
	else
		new_dir = ft_strdup(dir);
	if (access(new_dir, F_OK) == -1)
		return (print_error("bash: cd: d: No such file or directory\n", 0));
	update_envp_pwd(info, new_dir);
	chdir(new_dir);
	free(new_dir);
	return (0);
}

/*
 * Checks if the dir is valid and if so updates the envp pwd/oldpwd
 *
 * Arguments:
 * - dir - directory name
 * - info - info structure
 * 
 * Returns:
 * 0 on success, 1 on error
 */
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
		return (print_error("bash: cd: ", 0));//TODO:dir name
	update_envp_pwd(info, dirpath);
	chdir(dirpath);
	free(dirpath);
	return (0);
}

/*
 * Checks the num of arguments and if valid checks if abs or relative path
 *
 * Arguments:
 * - args - the command string to check
 * - info - info structure
 * 
 * Returns:
 * 0 on success, 1 on error
 */
int	handle_cd(t_list *args, t_info *info)
{
	char	*dir;
	t_list	*current;

	if (args->next && args->next->next)
		return (print_error("bash: cd: too many arguments\n", 0));
	else if (!args->next)
		return (dir_home(info));
	else
	{
		current = args->next;
		dir = (char *)current->content;
		if (!ft_strncmp(dir, "/", 1) || !ft_strncmp(dir, "~/", 2))
			return (dir_abs_path(info, dir));
		else
			return (dir_rel_path(info, dir));
	}
	return (0);
}
