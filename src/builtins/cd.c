/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/30 13:27:23 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	update_envp_pwd(t_info *info, char *newpwd)
{
	char	**current;
	char	*oldpwd;
	char	*tmp;

	oldpwd = info->pwd;
	current = info->envp;
	while (*current)
	{
		if (!ft_strncmp(*current, "PWD=", 4))
		{
			tmp = ft_strjoin("PWD=", newpwd);
			free(*current);
			*current = tmp;
		}
		else if (!ft_strncmp(*current, "OLDPWD=", 7))
		{
			tmp = ft_strjoin("OLDPWD=", oldpwd);
			free(*current);
			*current = tmp;
		}
		current++;
	}
	return (0);
}

void printenv(char **envp)
{
	char **current;
	current = envp;
	while (*current)
	{
		if (!ft_strncmp(*current, "PWD=", 4) || !ft_strncmp(*current, "OLDPWD=", 7))
			printf("%s\n", *current);
		current++;
	}
}

int	handle_cd(t_list *args, t_info *info)
{
	char *dir;
	char *cdpath;
	char *dirpath;
	char *tmp;
	char **cdpaths;
	t_list *current;

	if (args->next && args->next->next)
	{
		print_error("bash: cd: too many arguments\n", 0);
		return(1);
	}
	else if (!args->next) // no directory provided
	{
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
		update_envp_pwd(info, dirpath);
		return (0);
	}
	else
	{
		current = args->next;
		dir = (char *)current->content;
		if (!ft_strncmp(dir, "/", 1)) // absolute path
		{
			if (chdir(dir) == -1)
			{
				print_error("0 - chdir error\n", 0);
				return (1);
			}
			update_envp_pwd(info, dirpath);
			return(0);
		}
		else
		{
			cdpath = info->pwd;
			if (!*cdpath)
			{
				print_error("1 - path not available\n", 0);
				return(1);
			}
			tmp = ft_strjoin(cdpath, "/");
			dirpath = ft_strjoin(tmp, dir);
			free (tmp);
			if (chdir(dirpath) == -1)
			{
				print_error("2 - chdir error\n", 0);
				return (1);
			}
			update_envp_pwd(info, dirpath);
			free(dirpath);
			return(0);
		}
	}
	return (0);
}