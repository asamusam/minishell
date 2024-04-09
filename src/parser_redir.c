/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:31:52 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/09 14:48:26 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "stdbool.h"
#include <sys/stat.h>

/*
 * Prompts and writes the user input into a here-document.
 *
 * Arguments:
 * - delimiter — delimiter provided by the user.
 * - fd — file descriptor of an opened here-document.
 * 
 * Returns:
 * Zero if there was an error or SIGINT was received, one on success.
 */
int	get_input(char *delimiter, int fd)
{
	char	*line;

	line = readline("->");
	while (line && ft_strcmp(line, delimiter) && g_signal != SIGINT)
	{
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
			return (FAIL);
		free(line);
		line = readline("->");
	}
	if (line)
		free(line);
	else
		printf("minishell: warning: here-document delimited by end-of-file\n");
	if (g_signal == SIGINT)
		return (FAIL);
	return (SUCCESS);
}

/*
 * Opens a here-document and prompts the user input.
 *
 * Arguments:
 * - delimiter — delimiter provided by the user.
 * 
 * Returns:
 * File descriptor of the opened here-document or -1 on error.
 */
int	handle_insource(char *delimiter)
{
	int	fd;
	int	flag;
	int	mode;

	flag = O_CREAT | O_TRUNC | O_WRONLY;
	mode = S_IRUSR | S_IWUSR | S_IROTH;
	fd = open("minishell_heredoc.txt", flag, mode);
	if (fd == -1)
		return (-1);
	if (!get_input(delimiter, fd))
		return (-1);
	return (fd);
}

/*
 * Opens a file suitable for the given redirect type.
 *
 * Arguments:
 * - type — token type.
 * - file — file name or delimiter.
 * - cmd — command structure to write the file descriptor to.
 * 
 * Returns:
 * Zero if there was an error and one on success.
 */
int	handle_redirect(int type, char *file, t_command *cmd)
{
	int		fd;
	bool	in;
	bool	out;

	in = type == REDIRECT_IN || type == REDIRECT_INSOURCE;
	out = type == REDIRECT_OUT || type == REDIRECT_APPEND;
	if (in && cmd->file_in != -1 && close(cmd->file_in) == -1)
		return (FAIL);
	if (out && cmd->file_out != -1 && close(cmd->file_out) == -1)
		return (FAIL);
	if (type == REDIRECT_IN)
		fd = open(file, O_RDONLY);
	else if (type == REDIRECT_OUT)
		fd = open(file,  O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (type == REDIRECT_APPEND)
		fd = open(file,  O_WRONLY | O_CREAT | O_APPEND);
	else
		fd = handle_insource(file);
	if (fd == -1)
		return (FAIL);
	if (in)
		cmd->file_in = fd;
	else
		cmd->file_out = fd;
	return (SUCCESS);
}
