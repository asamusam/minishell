/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:41:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/10 14:46:11 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <errno.h>

/*
 * Allocates with malloc() and initializes a command structure.
 *
 * Arguments:
 * None.
 * 
 * Returns:
 * A structure of type t_command.
 */
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

/*
 * Allocates with malloc() the argument value and the node
 * containing it, then adds this new node to the list of arguments.
 * 
 * Arguments:
 * - args — pointer to the argument list
 * - value — the argument value
 * 
 * Returns:
 * Zero on success and one when memory allocation fails.
 */
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

/*
 * Allocates with malloc() and adds a new
 * node to the list of commands.
 * 
 * Arguments:
 * - commands — pointer to the command list
 * - cmd — the command structure to add to the list
 * 
 * Returns:
 * Zero on success and one when memory allocation fails.
 */
int	add_command(t_list **commands, t_command *cmd)
{
	t_list	*command;

	command = ft_lstnew(cmd);
	if (!command)
		return (FAIL);
	ft_lstadd_back(commands, command);
	return (SUCCESS);
}

/*
 * Iterates through the list of tokens in a group
 * and fills the command structure accordingly to the token types.
 * 
 * Arguments:
 * - group — pointer to the group structure
 * 
 * Returns:
 * Command structure or NULL on error.
 */
t_command	*get_command(t_list *group)
{
	t_command	*cmd;
	t_token		*tkn;
	t_list		*tkns;
	int			type;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	tkns = (t_list *)group->content;
	while (tkns)
	{
		tkn = (t_token *)tkns->content;
		if (is_expandable(tkn->type) && add_arg(&cmd->args, tkn->value) == FAIL)
			return (free_command_return_null(cmd));
		else if (REDIRECT_OUT <= tkn->type && tkn->type <= REDIRECT_INSOURCE)
		{
			type = tkn->type;
			while (!is_expandable(((t_token *)tkns->content)->type))
				tkns = tkns->next;
			if (handle_redirect(type, ((t_token *)tkns->content)->value, cmd))
				return (free_command_return_null(cmd));
		}
		tkns = tkns->next;
	}
	return (cmd);
}

/*
 * Turns a list of token groups into a list of commands.
 * 
 * Arguments:
 * - groups — pointer to the group list
 * 
 * Returns:
 * List of commands or NULL on error.
 */
t_list	*get_commands(t_list *groups)
{
	t_list		*commands;
	t_command	*command;

	commands = NULL;
	while (groups)
	{
		command = get_command(groups);
		if (!command || add_command(&commands, command) == FAIL)
		{
			if (errno)
				print_error("minishell parser", PERROR);
			if (command)
				free_command(command);
			return (free_commands_return_null(commands));
		}
		groups = groups->next;
	}
	return (commands);
}
