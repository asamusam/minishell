/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/15 06:06:11 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
int	update_envp_pwd(t_info *minishell, char *newpwd)
{
	char	*oldpwd;
	char	*key;
	t_list	*envp_list;

	envp_list = minishell->envp_list;
	oldpwd = ft_strdup(minishell->pwd);
	while (envp_list)
	{
		key = ((t_envp *)envp_list->content)->key;
		if (key && !ft_strncmp(key, "PWD", 3))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = ft_strdup(newpwd);
			minishell->pwd = ((t_envp *)envp_list->content)->value;
		}
		else if (key && !ft_strncmp(key, "OLDPWD", 6))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = oldpwd;
			minishell->oldpwd = ((t_envp *)envp_list->content)->value;
		}
		envp_list = envp_list->next;
	}
	update_envstr(minishell);
	return (SUCCESS);
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
int	dir_home(t_info *minishell)
{
	char	*dir;

	dir = minishell->home;
	if (dir == NULL)
		return (print_error("minishell: cd: HOME not set\n", 0));
	if (access(dir, F_OK) == -1)
		return (print_error("minishell: cd: d: No such file or directory\n", 0));
	update_envp_pwd(minishell, dir);
	chdir(dir);
	return (SUCCESS);
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
int	dir_abs_path(t_info *minishell, char *dir)
{
	char	*new_dir;
	
	if (dir[0] == '~')
	{
		if (!minishell->home)
			return(print_error("no home defined\n", 1));
		dir++;
		new_dir = ft_strjoin(minishell->home, dir);
	}
	else
		new_dir = ft_strdup(dir);
	free(dir);
	if (access(new_dir, F_OK) == -1)
		return (print_error("minishell: cd: d: No such file or directory\n", 0));
	update_envp_pwd(minishell, new_dir);
	chdir(new_dir);
	free(new_dir);
	return (SUCCESS);
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
int	dir_rel_path(t_info *minishell, char *dir)
{
	char	*dirpath;
	char	*cdpath;
	char	*tmp;

	cdpath = minishell->pwd;
	if (!cdpath)
		return (print_error("No path available\n", 0));
	tmp = ft_strjoin(cdpath, "/");
	dirpath = ft_strjoin(tmp, dir);
	free (tmp);
	free (dir);
	if (access(dirpath, F_OK) == -1)
	{
		free (dirpath);
		return (print_error("bash: cd: ", 0));//TODO:dir name
	}
	update_envp_pwd(minishell, dirpath);
	chdir(dirpath);
	free(dirpath);
	return (SUCCESS);
}

char	*check_last_dir_slash(char *dir, char *path)
{
	int	len;

	len = ft_strlen(path);
	if (path[len - 1] == '/')
	{
		dir = malloc(sizeof(char) * len);
		if (!dir)
		{
			print_error("malloc error\n", 0);
			return (NULL);
		}
		ft_strlcpy(dir, path, len);
	}
	else
		dir = ft_strdup(path);
	return (dir);
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
int	handle_cd(t_list *args, t_info *minishell)
{
	char	*dir = NULL;
	char	*path;
	t_list	*current;

	if (args->next && args->next->next)
		return (print_error("bash: cd: too many arguments\n", 0));
	else if (!args->next)
		return (dir_home(minishell));
	current = args->next;
	if (!ft_strcmp((char *)current->content, ".."))
	{
		return (SUCCESS); //TODO:
	}
	else
	{
		path = (char *)current->content;
		dir = check_last_dir_slash(dir, path);
		if (!ft_strncmp(dir, "/", 1) || !ft_strncmp(dir, "~/", 2))
			return (dir_abs_path(minishell, dir));
		else
			return (dir_rel_path(minishell, dir));
	}
	return (SUCCESS);
}
