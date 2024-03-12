/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_func_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:25:50 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/12 18:48:45 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// checks whether the given symbol is
// one of the following: space, horizontal tab,
// new line, vertical tab, form feed, or carriage return
int	ft_isspace(int c)
{
	return (c == 32 || (9 <= c && c <= 13));
}

// returns 1 of the passed symbol is 
// of token type WORD and 0 if it is not
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

// prints the contents of a token structure: 
// type, value, and length
void	print_token(void *arg)
{
	t_token	*token;

	token = (t_token *)arg;
	ft_printf("token type: %d\ntoken value: ", token->type);
	write(STDOUT_FILENO, token->value, token->len);
	ft_printf("\ntoken len: %d\n", token->len);
}
