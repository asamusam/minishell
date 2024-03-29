/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:41:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 20:42:21 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_command	*init_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->file_in = -1;
	command->file_out = -1;
	return (command);
}

int	add_arg(t_list **args, char *value)
{
	t_list	*arg;
	char	*copy;

	copy = ft_strdup(value);
	if (!copy)
		return (FAIL);
	arg = ft_lstnew(copy);
	if (!arg)
		return (FAIL);
	ft_lstadd_back(args, arg);
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

int	get_command(t_list *group, t_list **commands)
{
	t_command	*cmd;
	t_token		*tkn;
	t_list		*tkns;
	int			type;

	cmd = init_command();
	if (!cmd)
		return (print_error("minishell parser", PERROR));
	tkns = (t_list *)group->content;
	while (tkns)
	{
		tkn = (t_token *)tkns->content;
		if (is_expandable(tkn->type) && !add_arg(&cmd->args, tkn->value))
			return (free_command_return_fail(cmd));
		else if (REDIRECT_OUT <= tkn->type && tkn->type <= REDIRECT_INSOURCE)
		{
			type = tkn->type;
			while (!is_expandable(((t_token *)tkns->content)->type))
				tkns = tkns->next;
			if (!handle_redirect(type, ((t_token *)tkns->content)->value, cmd))
				return (free_command_return_fail(cmd));
		}
		tkns = tkns->next;
	}
	if (!add_command(commands, cmd))
		return (free_command_return_fail(cmd));
	return (SUCCESS);
}

// turns groups into a list of commands
t_list	*get_commands(t_list *groups)
{
	t_list	*commands;

	commands = NULL;
	while (groups)
	{
		if (!get_command(groups, &commands))
		{
			print_error("minishell parser", PERROR);
			return (free_commands_return_null(commands));
		}
		groups = groups->next;
	}
	return (commands);
}
