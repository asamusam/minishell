/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:29:41 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/23 14:23:47 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	redir_stdin(int fd, int pipe)
{
	int	redir;

	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	if (fd != -1)
		redir = fd;
	else
		redir = pipe;
	if (dup2(redir, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(redir) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	redir_stdout(int fd, int pipe)
{
	int	redir;

	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	if (fd != -1)
		redir = fd;
	else
		redir = pipe;
	if (dup2(redir, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(redir) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	backup_stdin_stdout(int *in, int *out, t_command *command)
{
	if (command->file_in)
	{
		*in = dup(STDIN_FILENO);
		if (*in == -1)
			return (print_error(DUP_ERROR, PERROR));
	}
	if (command->file_out)
	{
		*out = dup(STDOUT_FILENO);
		if (*out == -1)
			return (print_error(DUP_ERROR, PERROR));
	}
	return (SUCCESS);
}

int	restore_stdin(int original_stdin)
{
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdin) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	restore_stdout(int original_stdout)
{
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdout) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}
