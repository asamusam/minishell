/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 14:19:53 by mmughedd         ###   ########.fr       */
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
		return (print_error("minishell: cd: HOME not set", PERROR));
	if (access(dir, F_OK) == -1)
		return (print_error("minishell: cd: No such file or directory", PERROR));
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
	char	*tmp;
	
	if (dir[0] == '~')
	{
		if (!minishell->home)
			return(print_error("no home defined\n", 1));
		if (dir[1])
		{
			tmp = dir;
			tmp++;
			new_dir = ft_strjoin(minishell->home, tmp);
		}
		else
			new_dir = ft_strdup(minishell->home);
	}
	else
		new_dir = ft_strdup(dir);
	free(dir);
	if (access(new_dir, F_OK) == -1)
		return (print_error("minishell: cd ", PERROR));
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

int	last_lash_index(char *pwd)
{
	int	index;

	index = ft_strlen(pwd);
	index--;
	while (pwd[index] && index >= 0)
	{
		if (pwd[index] == '/')
			return (index);
		index--;
	}
	return (index);
}

char	*get_parent_dir(char *path)
{
	char	*dir;
	int		slash_i;

	if (!path)
		return (NULL); //TODO:
	slash_i = last_lash_index(path);
	if (slash_i == -1 || slash_i == 0)
		dir = ft_strdup(path);
	else
		dir = ft_substr(path, 0, slash_i);
	return(dir);
}

// char	*get_parent_dir(t_info *minishell)
// {
// 	char	*dir;
// 	int		slash_i;

// 	if (!minishell->pwd)
// 		return (NULL); //TODO:
// 	slash_i = last_lash_index(minishell->pwd);
// 	if (slash_i == -1 || slash_i == 0)
// 		dir = ft_strdup(minishell->pwd);
// 	else
// 		dir = ft_substr(minishell->pwd, 0, slash_i);
// 	return(dir);
// }

char	*check_last_dir_slash(char *dir, char *path)
{
	int	len;

	len = ft_strlen(path);
	if (path[--len] == '/')
		dir = ft_substr(path, 0, len);
	else
		dir = ft_strdup(path);
	return (dir);
}

char	*extended_path(t_info *minishell, char *path)
{
	char	*tmp;
	char	*new_dir = NULL;
	char	**dirs;
	int		i;
	// int		len;

	i = 0;
	if (path[0] == '~')
	{
		if (!minishell->home)
		{
			print_error("no home defined\n", STDERR);
			return (new_dir);
		}
		if (path[1])
		{
			tmp = path;
			tmp++;
			new_dir = ft_strjoin(minishell->home, tmp);
		}
		else
			new_dir = ft_strdup(minishell->home);
	}
	dirs = ft_split(path, '/');
	while (dirs[i])
	{
		if (!ft_strcmp(dirs[i], "."))
		{
			if (!new_dir)
				new_dir = ft_strdup(minishell->pwd);
		}
		else 
		{
			if (!ft_strcmp(dirs[i], ".."))
			{
				if (!new_dir)
					tmp = get_parent_dir(minishell->pwd);
				else
					tmp = get_parent_dir(new_dir);
			}
			else
			{
				if (!new_dir)
					tmp = ft_strdup(dirs[i]);
				else
					tmp = ft_strjoin(new_dir, dirs[i]);
			}
			if (dirs[i + 1])
			{
				new_dir = ft_strjoin(tmp, "/");
				free(tmp);
			}
			else
			{
				new_dir = ft_strdup(tmp);
				free(tmp);
			}
		}
		i++;
	}
	return (new_dir);
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
	char	*path = NULL;
	t_list	*current;

	if (args->next && args->next->next)
		return (print_error("minishell: cd: too many arguments", PERROR));
	else if (!args->next)
		return (dir_home(minishell));
	current = args->next;
	if (!ft_strcmp((char *)current->content, ".."))
	{
		dir = get_parent_dir(minishell->pwd);
		if (!dir)
			return (FAIL);
		else
			return (dir_abs_path(minishell, dir));
	}
	else if (!ft_strcmp((char *)current->content, "."))
		return (SUCCESS);
	else if (!ft_strcmp((char *)current->content, "./"))
		return (SUCCESS);
	else if (!ft_strcmp((char *)current->content, "/"))
		//TODO:
		return (dir_home(minishell));
	else
	{
		path = (char *)current->content;
		dir = check_last_dir_slash(dir, path);
		if (path[0] == '/' || path[0] == '~' || path[0] == '.')
		{
			//dir = extended_path(minishell, path);
			return (dir_abs_path(minishell, dir));
		}
		else
		{
			
			return (dir_rel_path(minishell, dir));
		}
	}
	return (SUCCESS);
}
