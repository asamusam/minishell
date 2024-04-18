/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/18 11:19:08 by mmughedd         ###   ########.fr       */
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

	status = SUCCESS;
	if (!is_buitin(command->args->content))
		status = handle_lst_cmd_process(pipet, command, minishell);
	else
	{
		status = handle_last_blt_redirection(pipet, command);
		status = handle_builtin(command, minishell);
		if (command->file_in >= 0)
		{
			if (dup2(pipet->orig_stdin, STDIN_FILENO) == -1)
				print_error(DUP2_ERROR, PERROR);
			if (close(pipet->orig_stdin) == -1)
				print_error(CLOSE_ERROR, PERROR);
		}
		else if (command->file_out >= 0)
		{
			if (dup2(pipet->orig_stdout, STDOUT_FILENO) == -1)
				print_error(DUP2_ERROR, PERROR);
			if (close(pipet->orig_stdout) == -1)
				print_error(CLOSE_ERROR, PERROR);
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
int	create_process(t_command *command, t_info *minishell, t_pipe *pipet)
{
	int		status;

	if (!command)
		return (FAIL);
	status = SUCCESS;
	if (pipe(pipet->pipefd) == -1)
		return (PIPE_ERROR, PERROR);
	if (!is_buitin(command->args->content))
		status = handle_cmd_process(pipet, command, minishell);
	else
		status = handle_bltn_process(pipet, command, minishell);
	return (status);
}

int	create_pipet(t_pipe **pipet)
{
	*pipet = malloc(sizeof(t_pipe));
	if (!*pipet)
		return (print_error(PIPET_ERROR, STDERR));
	(*pipet)->prev_pipe = dup(0);
	if ((*pipet)->prev_pipe == -1)
		return (print_error(DUP_ERROR, PERROR));
	(*pipet)->orig_stdin = dup(STDIN_FILENO);
	if ((*pipet)->orig_stdin == -1)
		return (print_error(DUP_ERROR, PERROR));
	(*pipet)->orig_stdout = dup(STDOUT_FILENO);
	if ((*pipet)->orig_stdout == -1)
		return (print_error(DUP_ERROR, PERROR));
	return (SUCCESS);
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
int	exec(t_list *commands, t_info *minishell)
{
	int			status;
	t_list		*current;
	t_pipe		*pipet;
	t_command	*cmd;

	status = SUCCESS;
	if (!commands->content)
		return (status);
	if (create_pipet(&pipet) == FAIL)
		return (FAIL);
	current = commands;
	while (!status && current && current->next && !minishell->exit_flag)
	{
		cmd = (t_command *)current->content;
		status = create_process(cmd, minishell, pipet);
		current = current->next;
	}
	cmd = (t_command *)current->content;
	if (!status && !minishell->exit_flag)
		status = last_process(cmd, minishell, pipet);
	free(pipet);
	return (status);
}
