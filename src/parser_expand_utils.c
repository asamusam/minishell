/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:18:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/27 17:29:36 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Merges two strings and increments the given index by 'i'.
 * 
 * Arguments:
 * - dst — double pointer to the destination string
 * - to_join — the string to be appended to 'dst'
 * - index — pointer to the index that will be incremented
 * - i — value by which to increment the given index
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	expand_special(char **dst, char *to_join, int *index, int i)
{
	if (!concat_strings(dst, to_join))
		return (FAIL);
	*index += i;
	return (SUCCESS);
}

/*
 * Frees a given string and returns zero.
 * 
 * Arguments:
 * - str — a string to be freed
 * 
 * Returns:
 * Zero.
 */
int	free_and_fail(char *str)
{
	if (str)
		free(str);
	return (FAIL);
}

/*
 * Changes the token value to 'new_value' after freeing the old one.
 * 
 * Arguments:
 * - token — pointer to a token
 * - new_value — a string to replace the old value
 * 
 * Returns:
 * Nothing.
 */
void	change_token_value(t_token *token, char *new_value)
{
	free(token->value);
	token->value = new_value;
}

/*
 * Merges the 'tokens' node into the 'merge' node and changes
 * the 'tokens' pointer to the resulting merged structure.
 * 
 * Arguments:
 * - tokens — double pointer to the current node in the token list
 * - token — double pointer to the token contained in the current node 
 * of the token list
 * - merge — pointer to the merging destination node of the token list
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	merge_tokens(t_list **tokens, t_token **token, t_list *merge)
{
	*tokens = merge_nodes(merge, *tokens);
	if (*tokens)
		*token = (t_token *)(*tokens)->content;
	else
		return (FAIL);
	return (SUCCESS);
}
