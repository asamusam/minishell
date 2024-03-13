/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:25:50 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/13 16:27:16 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Checks if a character is of token type WORD.
 *
 * Arguments:
 * - c — character to check.
 * 
 * Returns:
 * Positive value if the character is a WORD and zero if not.
 */
int	is_word(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (0);
	if (c == '\'' || c == '\"')
		return (0);
	if (ft_isspace(c) || !c)
		return (0);
	return (1);
}

/*
 * Allocates with malloc() and returns a pointer to a token structure.
 * 
 * Arguments:
 * - type — token type in range from 1 to 9 as defined in the header
 * - value — a pointer to the beginning of the token value
 * - len — length of the token in bytes
 * 
 * Returns:
 * A pointer to the filled token structure. NULL if the allocation fails.
 */
t_token	*create_token(int type, char *value, int len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->len = len;
	return (token);
}

/*
 * Allocates with malloc() and adds a new node to the token list, with
 * the content of the node being a pointer to the token.
 * The token is also allocated with malloc().
 * 
 * Arguments:
 * - token_lst — pointer to the beginning of the token list
 * - token — pointer to a token
 * 
 * Returns:
 * Pointer to the new node in token_lst or NULL if the allocation fails.
 */
t_list	*add_token(t_list **token_lst, t_token *token)
{
	t_list	*node;

	node = ft_lstnew(token);
	if (!node)
	{
		free(token);
		return (NULL);
	}
	ft_lstadd_back(token_lst, node);
	return (node);
}
