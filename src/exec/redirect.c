/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:50:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/12 10:37:14 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	handle_last_redirection(t_pipe *pipet, t_command *command)
{
	if (command->file_in >= 0)
	{
		dup2(command->file_in, STDIN_FILENO);
		close(command->file_in);
	}
	else
		dup2(pipet->prev_pipe, STDIN_FILENO);
	close(pipet->prev_pipe);
	if (command->file_out >= 0)
	{
		dup2(command->file_out, STDOUT_FILENO);
		close(command->file_out);
	}
	return (0);
}

int	handle_redirections(t_pipe *pipet, t_command *command)
{
	close (pipet->pipefd[0]);
	if (command->file_in >= 0)
	{
		dup2 (command->file_in, STDIN_FILENO);
		close(command->file_in);
	}
	else
		dup2 (pipet->prev_pipe, STDIN_FILENO);
	close(pipet->prev_pipe);
	if (command->file_out >= 0)
	{
		dup2 (command->file_out, STDOUT_FILENO);
		close(command->file_out);
	}
	else
		dup2 (pipet->pipefd[1], STDOUT_FILENO);
	close (pipet->pipefd[1]);
	return (0);
}

int	handle_blt_redirections(t_pipe *pipet, t_command *command)
{
	close (pipet->pipefd[0]);//
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
	return (0);
}
