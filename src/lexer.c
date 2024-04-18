/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 13:35:22 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/17 17:00:30 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
 * Determines the token type and calls a corresponding function 
 * to add the token to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	get_token(t_list **token_lst, char *line)
{
	if (*line == '|')
		return (add_pipe(token_lst, line));
	else if (*line == '<' && *(line + 1) == '<')
		return (add_redir_insource(token_lst, line));
	else if (*line == '<')
		return (add_redir_in(token_lst, line));
	else if (*line == '>' && *(line + 1) == '>')
		return (add_redir_append(token_lst, line));
	else if (*line == '>')
		return (add_redir_out(token_lst, line));
	else if (*line == '\'')
		return (add_field(token_lst, line));
	else if (*line == '\"')
		return (add_exp_field(token_lst, line));
	else if (ft_isspace(*line))
		return (add_separator(token_lst, line));
	else
		return (add_word(token_lst, line));
}

/*
 * Transforms a user input string into a list of tokens (logical units).
 *
 * Arguments:
 * - line — user input string
 * 
 * Returns:
 * List of type t_list with a t_token structure as a content of each node.
 */
t_list	*lexer(char *line)
{
	int		i;
	int		len;
	t_list	*token_lst;

	i = 0;
	token_lst = NULL;
	while (line[i])
	{
		len = get_token(&token_lst, line + i);
		if (len == -1)
		{
			if (token_lst)
				ft_lstclear(&token_lst, free_token);
			return (NULL);
		}
		i = i + len;
	}
	return (token_lst);
}
