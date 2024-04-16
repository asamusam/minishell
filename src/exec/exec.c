/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 15:08:56 by mmughedd         ###   ########.fr       */
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
		return (FAIL); //TODO:
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
				print_error("minishell: dup2 error", PERROR);
			if (close(pipet->orig_stdin) == -1)
				print_error("minishell: close error", PERROR);
		}
		else if (command->file_out >= 0)
		{
			if (dup2(pipet->orig_stdout, STDOUT_FILENO) == -1)
				print_error("minishell: dup2 error", PERROR);
			if (close(pipet->orig_stdout) == -1)
				print_error("minishell: close error", PERROR);
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
		return (FAIL); //TODO:
	status = SUCCESS;
	if (pipe(pipet->pipefd) == -1)
		return (print_error("minishell: pipe error", PERROR));
	if (!is_buitin(command->args->content))
		status = handle_cmd_prlsocess(pipet, command, minishell);
	else
		status = handle_bltn_process(pipet, command, minishell);
	return (status);
}

int	create_pipet(t_pipe **pipet)
{
	*pipet = malloc(sizeof(t_pipe));
	if (!*pipet)
		return (print_error("minishell exec", PERROR));
	(*pipet)->prev_pipe = dup(0);
	if ((*pipet)->prev_pipe == -1)
		return(print_error("minishell: dup2 error", PERROR));
	(*pipet)->orig_stdin = dup(STDIN_FILENO);
	if ((*pipet)->orig_stdin == -1)
		return(print_error("minishell: dup2 error", PERROR));
	(*pipet)->orig_stdout = dup(STDOUT_FILENO);
	if ((*pipet)->orig_stdout == -1)
		return(print_error("minishell: dup2 error", PERROR));
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
	int		status;
	t_list	*current = NULL;
	t_pipe	*pipet = NULL;

	status = SUCCESS;
	if (!commands || !commands->content || !((t_command *)(commands->content))->args)
		return (status); //TODO:
	if (create_pipet(&pipet) == FAIL)
		return (FAIL);
	current = commands;
	while (!status && current && current->next && !minishell->exit_flag)
	{
		status = create_process((t_command *)(current->content), minishell, pipet);
		current = current->next;
	}
	if (!status && !minishell->exit_flag)
		status = last_process((t_command *)(current->content), minishell, pipet);
	free(pipet);
	return (status);
}
