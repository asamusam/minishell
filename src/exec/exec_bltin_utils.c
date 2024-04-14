/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bltin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:46:53 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/14 12:30:10 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_bltn_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int	status;

	if (!command)
		return (FAIL);
	status = SUCCESS;
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		handle_blt_redirections(pipet, command);
		status = handle_builtin(command, info);
		if (g_signal == SIGINT) //TODO: check
			exit(FAIL);
		exit(status);
	}
	else
	{		
		close (pipet->pipefd[1]);
		close (pipet->prev_pipe);
		pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, NULL, 0);
	}
	return (status);
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
int	is_buitin(char *cmd)
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
int	handle_builtin(t_command *command, t_info *info)
{
	int		n;
	t_list	*args;

	args = command->args;
	n = is_buitin(args->content);
	if (n == 1)
		return (handle_echo(args));
	else if (n == 2)
		return (handle_cd(args, info));
	else if (n == 3)
		return (handle_pwd(info));
	else if (n == 4)
		return (handle_export(args, info));
	else if (n == 5)
		return (handle_unset(args, info));
	else if (n == 6)
		return (handle_env(info));
	else if (n == 7)
		return (handle_exit(args, info));
	return (0);
}

