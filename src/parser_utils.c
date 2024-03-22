/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:03:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/22 12:13:12 by asamuilk         ###   ########.fr       */
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
 * Frees all the memory associated with the list of tokens.
 * 
 * Arguments:
 * - arg — pointer to the token list
 * 
 * Returns:
 * Nothing.
 */
void	free_token_list(void *arg)
{
	t_list	*tokens;

	tokens = (t_list *)arg;
	ft_lstclear(&tokens, free_token);
}
