/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:31:52 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/23 14:25:02 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	run_child(t_command *command, t_info *minishell, int in, int out)
{
	int		status;

	if (redir_stdin(command->file_in, in) == FAIL)
		return (FAIL);
	if (redir_stdout(command->file_out, out) == FAIL)
		return (FAIL);
	if (command->args && !is_builtin(command->args->content))
		status = handle_input(command, minishell);
	else if (command->args)
		status = handle_builtin(command, minishell);
	else
		status = SUCCESS;
	return (status);
}

static int	child(t_command *command, t_info *msh, int i)
{
	int	ecode;

	if (msh->psize == 0)
		ecode = run_child(command, msh, -1, -1);
	else if (i > 0 && i < msh->psize)
	{
		ecode = run_child(command, msh, msh->pipes[i - 1][0], msh->pipes[i][1]);
		close(msh->pipes[i][0]);
		close(msh->pipes[i][1]);
	}
	else if (i == 0)
	{
		close(msh->pipes[i][0]);
		ecode = run_child(command, msh, -1, msh->pipes[i][1]);
		close(msh->pipes[i][1]);
	}
	else
	{
		ecode = run_child(command, msh, msh->pipes[i - 1][0], -1);
		close(msh->pipes[i - 1][0]);
	}
	exit(ecode);
}

pid_t	run_command(t_command *command, t_info *msh, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (pid);
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child(command, msh, i);
	}
	return (pid);
}
