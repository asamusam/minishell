/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/19 22:37:20 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	waiting_signal_handler(int signal)
{
	if (signal == SIGINT)
		printf("\n");
	else if (signal == SIGQUIT)
		printf("Quit (core dumped)\n");
}

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

pid_t	run_pipe_part(int i, int **pipes, t_list *commands, t_info *minishell)
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
		minishell->last_prc = run_pipe_part(i, pipes, commands, minishell);
		if (close_pipes(pipes, i, size - 1) == FAIL)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		commands = commands->next;
		i ++;
	}
	free_pipes(pipes, size);
	return (status);
}

void	wait_for_children(t_info *minishell)
{
	int	status;

	signal(SIGINT, waiting_signal_handler);
	signal(SIGQUIT, waiting_signal_handler);
	if (waitpid(minishell->last_prc, &status, 0) == -1)
		minishell->exit_code = print_error(WAIT_ERROR, PERROR);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
		status = 128 + WTERMSIG(status);
	while (wait(NULL) != -1)
		continue ;
	minishell->exit_code = status;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	exec_test(t_list *commands, t_info *minishell)
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
