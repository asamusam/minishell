/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:29:41 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/20 00:10:57 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	redir_stdin(int	*original_stdin, int fd, int pipe)
{
	int	redir;

	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin == -1)
		return (print_error(DUP_ERROR, PERROR));
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

int	redir_stdout(int *original_stdout, int fd, int pipe)
{
	int	redir;

	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdout == -1)
		return (print_error(DUP_ERROR, PERROR));
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

int	restore_stdin(int original_stdin, int fd, int pipe)
{
	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdin) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	restore_stdout(int original_stdout, int fd, int pipe)
{
	if (fd == -1 && pipe == -1)
		return (SUCCESS);
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdout) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}
