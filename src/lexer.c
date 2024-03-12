/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 13:35:22 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/12 19:20:02 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

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

int	get_token(t_list **token_lst, char *line, int i)
{
	if (line[i] == '|')
		return (add_pipe(token_lst, line + i));
	else if (line[i] == '<' && line[i + 1] == '<')
		return (add_redir_insource(token_lst, line + i));
	else if (line[i] == '<')
		return (add_redir_in(token_lst, line + i));
	else if (line[i] == '>' && line[i + 1] == '>')
		return (add_redir_append(token_lst, line + i));
	else if (line[i] == '>')
		return (add_redir_out(token_lst, line + i));
	else if (line[i] == '\'')
		return (add_field(token_lst, line + i));
	else if (line[i] == '\"')
		return (add_exp_field(token_lst, line + i));
	else if (ft_isspace(line[i]))
		return (add_separator(token_lst, line + i));
	else
		return (add_word(token_lst, line + i));
}

// in: user input line
// out: list of tokens
t_list	*lexer(char *line)
{
	int		i;
	int		add;
	t_list	*token_lst;

	i = 0;
	token_lst = NULL;
	while (line[i])
	{
		add = get_token(&token_lst, line, i);
		if (add == -1)
			break ;
		i = i + add;
	}
	return (token_lst);
}
