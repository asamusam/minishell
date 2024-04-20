/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 11:52:58 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
		return (print_error(HOME_ERROR, STDERR));
	if (access(dir, F_OK) == -1)
		return (print_error(DIR_ERROR, PERROR));
	update_envp_pwd(minishell, dir);
	chdir(dir);
	return (SUCCESS);
}

int	get_abs_path(char *path)
{
	if (access(path, F_OK) == -1)
		return (print_error(CD_ERROR, PERROR));
	chdir(path);
	return (SUCCESS);
}

int	get_rel_path(char *path, t_info *minishell)
{
	char	*home;
	char	*tmp;

	home = ft_strjoin(minishell->pwd, "/");
	tmp = ft_strjoin(home, path);
	free(home);
	if (access(tmp, F_OK) == -1)
		return (print_error(CD_ERROR, PERROR));
	chdir(tmp);
	free(tmp);
	return (SUCCESS);
}

int	getcwd_and_upudate(t_info *minishell)
{
	char	*dir;

	dir = malloc(sizeof(char) * PATH_MAX + 1);
	if (!dir)
		return (print_error(MALLOC_ERROR, PERROR));
	getcwd(dir, PATH_MAX);
	update_envp_pwd(minishell, dir);
	free(dir);
	return (SUCCESS);
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
	t_list	*current;

	if (args->next && args->next->next)
		return (print_error(ARGS_ERROR, STDERR));
	else if (!args->next)
		return (dir_home(minishell));
	current = args->next;
	if (((char *)current->content)[0] == '/')
	{
		if (get_abs_path((char *)current->content) == FAIL)
			return (FAIL);
	}
	else
	{
		if (get_rel_path((char *)current->content, minishell) == FAIL)
			return (FAIL);
	}
	if (getcwd_and_upudate(minishell) == FAIL)
		return (FAIL);
	return (SUCCESS);
}
