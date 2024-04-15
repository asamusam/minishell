/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/15 10:34:41 by mmughedd         ###   ########.fr       */
=======
/*   Updated: 2024/04/10 15:57:41 by asamuilk         ###   ########.fr       */
>>>>>>> free
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
<<<<<<< HEAD
int create_process(t_command *command, t_info *minishell, t_pipe *pipet)
=======
int	create_process(t_command *command, t_info *info, t_pipe *pipet)
>>>>>>> free
{
	int		status;

	if (!command)
		return (FAIL); //TODO:
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
	if (!commands || !commands->content || !((t_command *)(commands->content))->args)
		return (status); //TODO:
	pipet = malloc(sizeof(t_pipe));
	if (!pipet)
		return (print_error("minishell exec", PERROR)); // newline is added automatically by perror() + we can use constant instead of raw value (defined in minishell.h)
	pipet->prev_pipe = dup(0);
	pipet->orig_stdin = dup(STDIN_FILENO);
	pipet->orig_stdout = dup(STDOUT_FILENO);
	current = commands;
<<<<<<< HEAD
	while (current && current->next && !minishell->exit_flag)
=======
	if (current && current->next)
		info->is_multiple_proc = 1;
	while (current && current->next && !info->exit_code) // exit_flag?
>>>>>>> free
	{
		status = create_process((t_command *)(current->content), minishell, pipet);
		current = current->next;
<<<<<<< HEAD
	};
	if (!minishell->exit_flag)
		status = last_process((t_command *)(current->content), minishell, pipet);
=======
	}
	if (!info->exit_code) // exit_flag?
		status = last_process((t_command *)(current->content), info, pipet);
>>>>>>> free
	free(pipet);
	return (status);
}
