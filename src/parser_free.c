/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:38:26 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/18 21:32:49 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Frees all the memory associated with the command structure
 * and closes opened files.
 * 
 * Arguments:
 * - arg — pointer to the command
 * 
 * Returns:
 * Nothing.
 */
void	free_command(void *arg)
{
	t_command	*command;

	command = (t_command *)arg;
	if (command->file_in != -1)
		close(command->file_in);
	if (command->file_out != -1)
		close(command->file_out);
	ft_lstclear(&command->args, free);
	free(command);
}

/*
 * Frees all the memory associated with the command structure,
 * closes the files, and returns zero.
 * 
 * Arguments:
 * - command — pointer to the command
 * 
 * Returns:
 * Zero.
 */
t_command	*free_command_return_null(t_command *command)
{
	if (command)
		free_command(command);
	return (NULL);
}

/*
 * Frees all the memory associated with the list of commands
 * and returns NULL.
 * 
 * Arguments:
 * - commands — pointer to the list of commands
 * 
 * Returns:
 * NULL.
 */
t_list	*free_commands_return_null(t_list *commands)
{
	if (commands)
		ft_lstclear(&commands, free_command);
	return (NULL);
}

/*
 * Frees all the memory associated with the list of tokens
 * and returns NULL.
 * 
 * Arguments:
 * - tokens — pointer to the list of tokens
 * 
 * Returns:
 * NULL.
 */
t_list	*free_tokens_return_null(t_list *tokens, int *status, int value)
{
	if (tokens)
		ft_lstclear(&tokens, free_token);
	*status = value;
	return (NULL);
}

/*
 * Frees all the memory associated with the list of groups
 * and returns NULL.
 * 
 * Arguments:
 * - groups — pointer to the list of groups
 * 
 * Returns:
 * NULL.
 */
t_list	*free_groups_return_null(t_list *groups, int *status, int value)
{
	if (groups)
		ft_lstclear(&groups, free_token_list);
	*status = value;
	return (NULL);
}
