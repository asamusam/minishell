/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bltin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:46:53 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/18 15:30:47 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_bltn_ps(t_pipe *pipet, t_command *command, t_info *minishell)
{
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error(FORK_ERROR, PERROR));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redirections(pipet, command) == FAIL)
			exit(FAIL);
		exit(handle_builtin(command, minishell));
	}
	else
		return (handle_parent(pipet));
}

/*
 * Checks if the string provided is a built-in cmd
 *
 * Arguments:
 * - the command string to check
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */
int	is_builtin(char *cmd)
{
	if (!ft_strcmp("echo", cmd))
		return (1);
	else if (!ft_strcmp("cd", cmd))
		return (2);
	else if (!ft_strcmp("pwd", cmd))
		return (3);
	else if (!ft_strcmp("export", cmd))
		return (4);
	else if (!ft_strcmp("unset", cmd))
		return (5);
	else if (!ft_strcmp("env", cmd))
		return (6);
	else if (!ft_strcmp("exit", cmd))
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
int	handle_builtin(t_command *command, t_info *minishell)
{
	int		n;
	t_list	*args;

	args = command->args;
	n = is_builtin(args->content);
	if (n == 1)
		return (handle_echo(args));
	else if (n == 2)
		return (handle_cd(args, minishell));
	else if (n == 3)
		return (handle_pwd(minishell));
	else if (n == 4)
		return (handle_export(args, minishell));
	else if (n == 5)
		return (handle_unset(args, minishell));
	else if (n == 6)
		return (handle_env(minishell));
	else if (n == 7)
		return (handle_exit(args, minishell));
	return (0);
}
