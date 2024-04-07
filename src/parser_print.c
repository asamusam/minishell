/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:30:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 20:36:59 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Prints the contents of the group of tokens.
 *
 * Arguments:
 * - arg — a node in the list of groups.
 * 
 * Returns:
 * Nothing.
 */
void	print_group(void *arg)
{
	t_list	*group;

	group = (t_list *)arg;
	printf("----GROUP-----\n");
	ft_lstiter(group, print_token);
}

/*
 * Prints the contents of the command structure.
 *
 * Arguments:
 * - arg — a structure of type t_command.
 * 
 * Returns:
 * Nothing.
 */
void	print_command(void *arg)
{
	t_command	*command;
	t_list		*args;

	command = (t_command *)arg;
	args = (t_list *)command->args;
	printf("---COMMAND---\nfile in: %d\nfile out: %d\nArguments:\n", \
			command->file_in, command->file_out);
	while (args)
	{
		printf("%s\n", (char *)args->content);
		args = args->next;
	}
}
