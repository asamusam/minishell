/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:50:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/18 11:23:17 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	redir_in(t_command *command)
{
	if (dup2(command->file_in, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(command->file_in) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	redir_out(t_command *command)
{
	if (dup2(command->file_out, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(command->file_out) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	handle_last_redirection(t_pipe *pipet, t_command *command)
{
	int	status;

	status = SUCCESS;
	if (!status && command->file_in >= 0)
		status = redir_in(command);
	else
	{
		if (!status && dup2(pipet->prev_pipe, STDIN_FILENO) == -1)
			return (print_error(DUP2_ERROR, PERROR));
	}
	if (!status && close(pipet->prev_pipe))
		return (print_error(CLOSE_ERROR, PERROR));
	if (!status && command->file_out >= 0)
		status = redir_out(command);
	return (status);
}

int	handle_last_blt_redirection(t_pipe *pipet, t_command *command)
{
	int	status;

	status = SUCCESS;
	if (!status && command->file_in >= 0)
		status = redir_in(command);
	if (!status && close(pipet->prev_pipe))
		return (print_error(CLOSE_ERROR, PERROR));
	if (!status && command->file_out >= 0)
		status = redir_out(command);
	return (status);
}

int	handle_redirections(t_pipe *pipet, t_command *command)
{
	int	status;

	status = SUCCESS;
	if (!status && close(pipet->pipefd[0]) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	if (!status && command->file_in >= 0)
		status = redir_in(command);
	else
	{
		if (!status && dup2(pipet->prev_pipe, STDIN_FILENO) == -1)
			return (print_error(DUP2_ERROR, PERROR));
	}
	if (!status && close(pipet->prev_pipe) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	if (!status && command->file_out >= 0)
		status = redir_out(command);
	else
	{
		if (!status && dup2(pipet->pipefd[1], STDOUT_FILENO) == -1)
			return (print_error(DUP2_ERROR, PERROR));
	}
	if (!status && close (pipet->pipefd[1]) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (status);
}
