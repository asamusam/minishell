/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:16:41 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/20 21:01:51 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_syntax(t_list *tokens)
{
	t_token	*token;
	int		prev_type;

	prev_type = 0;
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if ((!prev_type || prev_type == PIPE) && token->type == PIPE)
			return (print_error(SYNTAX_ERROR, STDERR));
		else if (REDIRECT_OUT <= prev_type && prev_type <= REDIRECT_INSOURCE && \
				REDIRECT_OUT <= token->type && token->type <= PIPE)
			return (print_error(SYNTAX_ERROR, STDERR));
		if (token->type != SEPARATOR)
			prev_type = token->type;
		tokens = tokens->next;
	}
	if (prev_type == PIPE || \
		(REDIRECT_OUT <= prev_type && prev_type <= REDIRECT_INSOURCE))
		return (print_error(SYNTAX_ERROR, STDERR));
	else
		return (1);
}

t_list	*split_groups(t_list *tokens)
{
	t_list	*groups;
	t_list	*group;
	t_list	*pipe;

	groups = NULL;
	group = tokens;
	while (tokens)
	{
		if (!tokens->next)
			ft_lstadd_back(&groups, ft_lstnew(group));
		else if (((t_token *)tokens->next->content)->type == PIPE)
		{
			pipe = tokens->next;
			tokens->next = NULL;
			ft_lstadd_back(&groups, ft_lstnew(group));
			tokens = pipe->next;
			group = pipe->next;
			ft_lstdelone(pipe, free_token);
			continue ;
		}
		tokens = tokens->next;
	}
	return (groups);
}

t_list	*parser(t_list *tokens, t_info *minishell)
{
	t_list	*commands;
	t_list	*groups;

	(void)minishell;
	commands = NULL;
	if (!check_syntax(tokens))
	{
		ft_lstclear(&tokens, free_token);
		return (NULL);
	}
	groups = split_groups(tokens);
	if (!groups)
	{
		ft_lstclear(&tokens, free_token);
		return (NULL);
	}
	// ft_lstiter(groups, expand);
	ft_lstiter(groups, print_group);
	ft_lstclear(&groups, free_groups);
	return (commands);
}
