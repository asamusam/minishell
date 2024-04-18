/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/18 14:41:46 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	run_builtin(t_command *command, t_info *minishell)
{
	int		original_stdin;
	int		original_stdout;
	int		status;

	if (redir_stdin(&original_stdin, command->file_in) == FAIL)
		return (FAIL);
	if (redir_stdout(&original_stdout, command->file_out) == FAIL)
		return (FAIL);
	status = handle_builtin(command, minishell);
	if (restore_stdin(original_stdin, command->file_in) == FAIL)
		return (FAIL);
	if (restore_stdout(original_stdout, command->file_out) == FAIL)
		return (FAIL);
	return (status);
}

int	run_single_command(t_list *commands, t_info *minishell)
{
	int			status;
	t_command	*command;

	command = (t_command *)commands->content;
	if (is_builtin(command->args->content))
		status = run_builtin(command, minishell);
	else
		status = run_command(command, minishell, -1, -1);
	return (status);
}

int	run_pipe_segment(int i, int **pipes, t_list *commands, t_info *minishell)
{
	t_command	*command;
	int			status;

	command = (t_command *)commands->content;
	if (i > 0 && commands->next)
		status = run_command(command, minishell, pipes[i - 1][0], pipes[i][1]);
	else if (i == 0)
		status = run_command(command, minishell, -1, pipes[i][1]);
	else
		status = run_command(command, minishell, pipes[i - 1][0], -1);
	return (status);
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
	while (commands && !status)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes || !pipes[i])
			return (free_pipes_return_fail(pipes, i, MALLOC_ERROR));
		if (pipe(pipes[i]) == -1)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		status = run_pipe_segment(i, pipes, commands, minishell);
		if (close_pipes(pipes, i, size - 1) == FAIL)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		commands = commands->next;
		i ++;
	}
	free_pipes(pipes, i);
	return (status);
}

int	exec_test(t_list *commands, t_info *minishell)
{
	int	status;

	if (!((t_command *)commands->content)->args)
		return (SUCCESS);
	if (!commands->next)
		status = run_single_command(commands, minishell);
	else
		status = run_pipeline(commands, minishell);
	return (status);
}
