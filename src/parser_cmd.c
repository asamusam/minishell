/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:41:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/28 21:00:37 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "stdbool.h"

void	init_command(t_command *command)
{
	command->args = NULL;
	command->file_in = -1;
	command->file_out = -1;
}

void	print_command(void *arg)
{
	t_command	*command;
	t_list		*args;

	command = (t_command *)arg;
	args = (t_list *)command->args;
	printf("---COMMAND---\nfile in: %d\n file out: %d\nArguments: ", \
			command->file_in, command->file_out);
	while (args)
	{
		printf("%s ", (char *)args->content);
		args = args->next;
	}
	write(STDOUT_FILENO, "\n", 1);
}

int	add_arg(t_list **args, char *value)
{
	t_list	*arg;
	char	*copy;

	copy = ft_strdup(value);
	if (!copy)
		return (FAIL);
	arg = ft_lstnew(value);
	if (!arg)
		return (FAIL);
	ft_lstadd_back(args, arg);
	return (SUCCESS);
}

int	handle_insource(char *delimiter);


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

int	add_command(t_list **commands, t_command *cmd)
{
	t_list	*command;

	command = ft_lstnew(cmd);
	if (!command)
		return (FAIL);
	ft_lstadd_back(commands, command);
	return (SUCCESS);
}

int	get_command(t_list *group, t_list *commands)
{
	t_command	*cmd;
	t_token		*tkn;
	t_list		*tokens;

	init_command(cmd);
	tokens = (t_list *)group->content;
	while (tokens)
	{
		tkn = (t_token *)tokens->content;
		if (is_expandable(tkn->type) && !add_arg(&cmd->args, tkn->value))
			return (print_error("minishell parser", PERROR));
		tokens = tokens->next;
	}
	if (!add_command(commands, cmd))
		return (FAIL);
	return (SUCCESS);
}

// turns groups into a list of commands
t_list	*get_commands(t_list *groups)
{
	t_list	*commands;

	while (groups)
	{
		if (!get_command(groups, commands))
			return (NULL);
		groups = groups->next;
	}
	return (commands);
}
