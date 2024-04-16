/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:50:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 15:15:19 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	redir_in(t_command *command)
{
	if (dup2(command->file_in, STDIN_FILENO) == -1)
		return(print_error("minishell: dup2 error", PERROR));
	if (close(command->file_in) == -1)
		return(print_error("minishell: close error", PERROR));
	return (SUCCESS);
}

int	redir_out(t_command *command)
{
	if (dup2(command->file_out, STDOUT_FILENO) == -1)
		return(print_error("minishell: dup2 error", PERROR));
	if (close(command->file_out) == -1)
		return(print_error("minishell: close error", PERROR));
	return (SUCCESS);
}

int	handle_last_redirection(t_pipe *pipet, t_command *command)
{
	int	status;

	status = SUCCESS;
	if (command->file_in >= 0)
		status = redir_in(command);
	else
	{
		if(dup2(pipet->prev_pipe, STDIN_FILENO) == -1)
			return(print_error("minishell: dup2 error", PERROR));
	}
	if (close(pipet->prev_pipe) && !status)
		return(print_error("minishell: close error", PERROR));
	if (command->file_out >= 0 && !status)
		status = redir_out(command);
	return (status);
}

int	handle_redirections(t_pipe *pipet, t_command *command)
{
	int	status;

	status = SUCCESS;
	close (pipet->pipefd[0]);
	if (command->file_in >= 0)
		status = redir_in(command);
	else
	{
		if(dup2(pipet->prev_pipe, STDIN_FILENO) == -1)
			return(print_error("minishell: dup2 error", PERROR));
	}
	if (close(pipet->prev_pipe) && !status)
		return(print_error("minishell: close error", PERROR));
	if (command->file_out >= 0 && !status)
		status = redir_out(command);
	else
	{
		if(dup2(pipet->pipefd[1], STDOUT_FILENO) == -1)
			return(print_error("minishell: dup2 error", PERROR));
	}
	if (close (pipet->pipefd[1]) == -1)
		return(print_error("minishell: close error", PERROR));
	return (status);
}

int	handle_blt_redirections(t_pipe *pipet, t_command *command)
{
	close (pipet->pipefd[0]);
	if (command->file_out >= 0)
	{
		dup2 (command->file_out, STDOUT_FILENO);
		close(command->file_out);
	}
	else
		dup2 (pipet->pipefd[1], STDOUT_FILENO);
	close (pipet->pipefd[1]);
	if (command->file_in >= 0)
	{
		dup2 (command->file_in, STDIN_FILENO);
		close(command->file_in);
	}
	close(pipet->prev_pipe);
	return (SUCCESS);
}
