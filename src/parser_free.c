/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:38:26 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/28 19:39:32 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Frees all the memory associated with the command structure.
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
	ft_lstclear(&command->args, free);
}

t_list	*free_commands_return_null(t_list *commands)
{
	if (commands)
		ft_lstclear(&commands, free_command);
	return (NULL);
}

t_list	*free_tokens_return_null(t_list *tokens)
{
	if (tokens)
		ft_lstclear(&tokens, free_token);
	return (NULL);
}

t_list	*free_groups_return_null(t_list *groups)
{
	if (groups)
		ft_lstclear(&groups, free_token_list);
	return (NULL);
}
