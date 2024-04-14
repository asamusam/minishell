/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/14 13:50:55 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Create last process, close remaining pipes, executes cmd
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */
int	last_process(t_command *command, t_info *minishell, t_pipe *pipet)
{
	int		status;

	if (!command)
		return (FAIL);
	status = SUCCESS;
	if (!is_buitin(command->args->content))
		status = handle_lst_cmd_process(pipet, command, minishell);
	else
	{
		handle_last_redirection(pipet, command);
		status = handle_builtin(command, minishell);
		if (command->file_in >= 0)
		{
			if (dup2(pipet->orig_stdin, STDIN_FILENO) == -1)
				print_error("dup2 error\n", 0);
			if (close(pipet->orig_stdin) == -1)
				print_error("close error\n", 0);
		}
		else if (command->file_out >= 0)
		{
			if (dup2(pipet->orig_stdout, STDOUT_FILENO) == -1)
				print_error("dup2 error\n", 0);
			if (close(pipet->orig_stdout) == -1)
				print_error("close error\n", 0);
		}
	}
	return (status);
}

/*
 * Create processes, pipes and executes commands
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */
int create_process(t_command *command, t_info *minishell, t_pipe *pipet)
{
	int		status;

	if (!command)
		return (FAIL);
	status = SUCCESS;
	if (pipe(pipet->pipefd) == -1)
		return (print_error("Pipe error\n", 0));
	if (!is_buitin(command->args->content))
		status = handle_cmd_process(pipet, command, minishell);
	else
		status = handle_bltn_process(pipet, command, minishell);
	return (status);
}

/*
 * Loops through commands list and creates processes accordingly
 *
 * Arguments:
 * - commands - list of commands/processes
 * - info - info structure
 * 
 * Returns:
 * Status
 */
int		exec(t_list *commands, t_info *minishell)
{
	int		status;
	t_list	*current;
	t_pipe	*pipet;

	status = SUCCESS;
	if (!commands || !commands->content)
		return (status); //TODO:
	pipet = malloc(sizeof(t_pipe));
	if (!pipet)
		return (print_error("malloc error\n", 0));
	pipet->prev_pipe = dup(0);
	pipet->orig_stdin = dup(STDIN_FILENO);
	pipet->orig_stdout = dup(STDOUT_FILENO);
	current = commands;
	while (current && current->next && !minishell->exit_code)
	{
		status = create_process((t_command *)(current->content), minishell, pipet);
		current = current->next;
	};
	if (!minishell->exit_code)
		status = last_process((t_command *)(current->content), minishell, pipet);
	free(pipet);
	return (status);
}
