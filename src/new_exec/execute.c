/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/19 19:10:34 by asamuilk         ###   ########.fr       */
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

void	run_pipe_segment(int i, int **pipes, t_list *commands, t_info *minishell)
{
	t_command	*command;

	command = (t_command *)commands->content;
	if (i > 0 && commands->next)
		minishell->processes[i] = run_command(command, minishell, pipes[i - 1][0], pipes[i][1]);
	else if (i == 0)
		minishell->processes[i] = run_command(command, minishell, -1, pipes[i][1]);
	else
		minishell->processes[i] = run_command(command, minishell, pipes[i - 1][0], -1);
}

int	wait_last(t_info *minishell, int last);

int	run_pipeline(t_list *commands, t_info *minishell)
{
	int	**pipes;
	int	i;
	int	status;
	int	size;
	int	child_status;

	i = 0;
	status = SUCCESS;
	size = ft_lstsize(commands);
	pipes = malloc(sizeof(int *) * size);
	minishell->processes = malloc(sizeof(int) * size);
	while (commands)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes || !pipes[i])
			return (free_pipes_return_fail(pipes, i, MALLOC_ERROR));
		if (pipe(pipes[i]) == -1)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		run_pipe_segment(i, pipes, commands, minishell);
		if (close_pipes(pipes, i, size - 1) == FAIL)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		commands = commands->next;
		i ++;
	}
	signal(SIGINT, SIG_IGN);
	status = wait_last(minishell, size - 1);
	i = 0;
	while (i < size)
	{
		waitpid(minishell->processes[i++], &child_status, 0);
		if (WIFSIGNALED(child_status))
		{
			if (WTERMSIG(child_status) == SIGINT)
				printf("\n");
			else if (WTERMSIG(child_status) == SIGQUIT)
				printf("Quit (core dumped)\n");
		}
	}
	free(minishell->processes);
	free_pipes(pipes, size);
	signal(SIGINT, signal_handler);
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
