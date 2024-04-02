/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:31:52 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/01 18:22:05 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "stdbool.h"

int	handle_insource(char *delimiter)
{
	int					fd;
	char				*line;

	fd = open("minishell_heredoc.txt", O_CREAT | O_TRUNC | O_WRONLY);
	if (fd == -1)
		return (-1);
	write(STDIN_FILENO, "->", 2);
	line = ft_getline(STDIN_FILENO);
	while (line && ft_strcmp(line, delimiter))
	{
		if (write(fd, line, ft_strlen(line)) == -1)
			return (-1);
		free(line);
		write(STDIN_FILENO, "->", 2);
		line = ft_getline(STDIN_FILENO);
	}
	free(line);
	return (fd);
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
	else if (type == REDIRECT_INSOURCE)
		fd = handle_insource(file);
	if (fd == -1)
		return (FAIL);
	if (in)
		cmd->file_in = fd;
	else
		cmd->file_out = fd;
	return (SUCCESS);
}
