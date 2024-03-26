/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:18:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/26 16:31:35 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Joins two token structures by concatenating the value of
 * 'token' to the value of 'merge', assigns the resulting string
 * to 'merge', and deletes the node containing 'token'.
*/


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
