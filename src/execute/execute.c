/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/20 00:36:12 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	run_builtin(t_command *command, t_info *minishell)
{
	int		original_stdin;
	int		original_stdout;
	int		status;

	if (redir_stdin(&original_stdin, command->file_in, -1) == FAIL)
		return (FAIL);
	if (redir_stdout(&original_stdout, command->file_out, -1) == FAIL)
		return (FAIL);
	status = handle_builtin(command, minishell);
	if (restore_stdin(original_stdin, command->file_in, -1) == FAIL)
		return (FAIL);
	if (restore_stdout(original_stdout, command->file_out, -1) == FAIL)
		return (FAIL);
	return (status);
}

void	run_single_command(t_list *commands, t_info *minishell)
{
	t_command	*command;

	command = (t_command *)commands->content;
	if (is_builtin(command->args->content))
	{
		minishell->exit_code = run_builtin(command, minishell);
		minishell->last_prc = 0;
	}
	else
		minishell->last_prc = run_command(command, minishell, -1, -1);
}

pid_t	run_pipe(int i, int **pipes, t_list *commands, t_info *minishell)
{
	t_command	*command;
	pid_t		pid;

	command = (t_command *)commands->content;
	if (i > 0 && commands->next)
		pid = run_command(command, minishell, pipes[i - 1][0], pipes[i][1]);
	else if (i == 0)
		pid = run_command(command, minishell, -1, pipes[i][1]);
	else
		pid = run_command(command, minishell, pipes[i - 1][0], -1);
	return (pid);
}

int	run_pipeline(t_list *commands, t_info *minishell)
{
	int	**pipes;
	int	i;
	int	status;
	int	size;

	i = 0;
	status = SUCCESS;
	size = ft_lstsize(commands);
	pipes = malloc(sizeof(int *) * size);
	while (commands)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes || !pipes[i])
			return (free_pipes_return_fail(pipes, i, MALLOC_ERROR));
		if (pipe(pipes[i]) == -1)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		minishell->last_prc = run_pipe(i, pipes, commands, minishell);
		if (close_pipes(pipes, i, size - 1) == FAIL)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		commands = commands->next;
		i ++;
	}
	free_pipes(pipes, size);
	return (status);
}

void	execute(t_list *commands, t_info *minishell)
{
	if (!((t_command *)commands->content)->args && !commands->next)
	{
		minishell->exit_code = SUCCESS;
		return ;
	}
	if (!commands->next)
		run_single_command(commands, minishell);
	else
		run_pipeline(commands, minishell);
	if (minishell->last_prc)
		wait_for_children(minishell);
}
