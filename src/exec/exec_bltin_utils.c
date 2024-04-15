/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bltin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:46:53 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 00:10:29 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_bltn_process(t_pipe *pipet, t_command *command, t_info *minishell)
{
	int		child_status;
	void	(*parent_handler)(int);

	if (!command)
		return (FAIL);
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_blt_redirections(pipet, command);
		exit(handle_builtin(command, minishell));
	}
	else
	{
		parent_handler = signal(SIGINT, SIG_IGN);
		close (pipet->pipefd[1]);
		close (pipet->prev_pipe);
		pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, &child_status, 0);
		signal(SIGINT, parent_handler);
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		else
		{
			if (WTERMSIG(child_status) == SIGINT)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (WTERMSIG(child_status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
			return (128 + WTERMSIG(child_status));
		}
	}
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
int	handle_builtin(t_command *command, t_info *minishell)
{
	int		n;
	t_list	*args;

	args = command->args;
	n = is_buitin(args->content);
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

