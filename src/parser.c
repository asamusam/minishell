/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:16:41 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/18 16:06:04 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_pipes(t_list *tokens)
{
	t_token	*token;
	int		prev_type;

	prev_type = 0;
	while (tokens)
	{
		if (tokens->content)
		{
			token = (t_token *)tokens->content;
			if (token->type == PIPE && (!prev_type || prev_type == PIPE))
				return (print_error(PIPE_SYNTAX_ERROR, STDERR));
			if (token->type != SEPARATOR)
				prev_type = token->type;
		}
		tokens = tokens->next;
	}
	if (prev_type == PIPE)
		return (print_error(PIPE_SYNTAX_ERROR, STDERR));
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
	if (!check_pipes(tokens))
		return (NULL);
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
			ft_lstdelone(pipe, free);
			continue ;
		}
		tokens = tokens->next;
	}
	return (groups);
}

t_list	*parser(t_list *tokens)
{
	t_list	*commands;
	t_list	*groups;

	commands = NULL;
	groups = split_groups(tokens);
	if (!groups)
		return (NULL);
	ft_lstiter(groups, print_group);
	ft_lstclear(&groups, free_groups);
	return (commands);
}
