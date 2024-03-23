/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:03:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/23 21:25:28 by asamuilk         ###   ########.fr       */
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

/*
 * Checks if the given token type is expandable, i.e.
 * belongs to one of these types: WORD, FIELD, or EXP_FIELD.
 * 
 * Arguments:
 * - type — token type
 * 
 * Returns:
 * One if the given type is expandable and zero if not.
 */
int	is_expandable(int type)
{
	if (type == WORD || type == FIELD || type == EXP_FIELD)
		return (1);
	return (0);
}


char	*get_envp_value(char *key, int len, t_info *minishell)
{
	t_list	*vars;
	t_envp	*var;

	if (!len)
		return (NULL);
	vars = minishell->envp_list;
	while (vars)
	{
		var = (t_envp *)vars->content;
		if (!ft_strncmp(key, var->key, len) && !var->key[len])
			return (ft_strdup(var->value));
		vars = vars->next;
	}
	return (NULL);
}