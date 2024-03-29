/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:18:54 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 17:37:57 by asamuilk         ###   ########.fr       */
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

