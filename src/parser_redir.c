/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:31:52 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/02 17:54:54 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "stdbool.h"
#include <sys/stat.h>
#include <errno.h>

int	handle_insource(char *delimiter)
{
	int		fd;
	char	*line;
	size_t	delim_len;

	fd = open("minishell_heredoc.txt", O_CREAT | O_TRUNC | O_WRONLY, \
									S_IRUSR | S_IWUSR | S_IROTH);
	if (fd == -1)
		return (-1);
	write(STDIN_FILENO, "->", 2);
	line = ft_getline(STDIN_FILENO);
	delim_len = ft_strlen(delimiter);
	while (line)
	{
		if ((!ft_strncmp(line, delimiter, delim_len) && (line[delim_len] == '\n' || !line[delim_len])))
		{
			free(line);
			return (fd);
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			return (-1);
		free(line);
		write(STDIN_FILENO, "->", 2);
		line = ft_getline(STDIN_FILENO);
	}
	errno = ENOMSG;
	return (-1);
}

// TODO: file can be a relative path
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
		fd = open(file, O_CREAT | O_TRUNC);
	else if (type == REDIRECT_APPEND)
		fd = open(file, O_APPEND);
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
