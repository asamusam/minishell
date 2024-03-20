/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 09:35:06 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/20 14:39:27 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/exec.h"

/*
 * Checks if the string provided is a built-in cmd
 *
 * Arguments:
 * - the command string to check
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */

int	is_buitin(char *cmd)
{
	if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
		return (1);
	else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
		return (2);
	else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
		return (3);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		return (4);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		return (5);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		return (6);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		return (7);
	return (0);
}

/*
 * Calls the handler function depending on the built-in command
 *
 * Arguments:
 * - command struct
 * - t_info struct
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */

int	handle_builtin(t_command *command,  t_info *info) // TODO: check which builtin needs *info
{
	int		n;
	t_list	*args;

	args = command->args;
	n = is_buitin(args->content);
	if (n == 1)
		handle_echo(args);
	else if (n == 2)
		handle_cd(args, info);
	else if (n == 3)
		handle_pwd();
	else if (n == 4)
		handle_export(args, info);
	else if (n == 5)
		handle_unset(args);
	else if (n == 6)
		handle_env(info);
	else if (n == 7)
		handle_exit(args);
	exit(0); // Exit child process after handling the built-in command
	// TODO: handle errors/return
	return(0);
}

/*
 * Locates and returns line in envp 
 *
 * Arguments:
 * - key: start of the line (f.i. PATH)
 * - envp strings
 * 
 * Returns:
 * line without initial key
 */

char	*find_envp_line(char *key, char **envp)
{
	int	len;

	len = ft_strlen(key);
	while (ft_strncmp(key, *envp, len))
		envp++;
	return (*envp + ++len);
}