/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:08:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/19 17:38:56 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_expandable(int type)
{
	if (type == WORD)
		return (1);
	if (type == FIELD)
		return (1);
	if (type == EXP_FIELD)
		return (1);
	return (0);
}

int	expand_token(t_token *token);

void	join_tokens(t_list *to_join)
{
	int		len;
	t_token	*token;
	t_list	*i;

	if (ft_lstsize(to_join) <= 1)
		return ;
	len = 0;
	i = to_join;
	while (i)
	{
		token = (t_token *)i->content;
		len += token->len;
		i = i->next;
	}
	token = (t_token *)to_join->content;
	token->len = len;
	to_join->next = ft_lstlast(to_join)->next;
	return (to_join);
}

int	expand(t_list *tokens)
{
	int		prev;
	t_token	*token;
	t_list	*to_join;

	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (!is_expandable(token->type))
		{
			tokens = tokens->next;
			continue ;
		}
		to_join = NULL;
		while (tokens && is_expandable(token->type))
		{
			if (prev != REDIRECT_INSOURCE)
				expand_token(token);
			ft_lstadd_back(to_join, token);
			tokens = tokens->next;
			if (tokens)
				token = (t_token *)tokens->content;
		}
		join_tokens(to_join);
	}
	return (1);
}
