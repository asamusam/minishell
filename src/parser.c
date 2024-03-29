/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:16:41 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 21:29:27 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Checks if there is a syntax error related to pipes and redirections
 * in tokens and prints a message to the standard error if there is.
 * 
 * Arguments:
 * - tokens — list of tokens
 * 
 * Returns:
 * Zero if there is an error and one if there is not.
 */
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

/*
 * Splits a token list into groups based on pipes.
 *
 * Arguments:
 * - tokens — list of tokens
 * 
 * Returns:
 * List of groups, each group containing a list of tokens,
 * representing a part of the original list of tokens.
 */
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

/*
 * Goes through a list of groups and expands everything
 * that has to be expanded (dollar signs and concatenation)
 * in the contained token lists.
 * 
 * Arguments:
 * - groups — pointer to the list of groups
 * - minishell — general info structure
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	expand_groups(t_list *groups, t_info *minishell)
{
	t_list	*temp;

	temp = groups;
	while (temp)
	{
		if (!expand((t_list *)temp->content, minishell))
			return (FAIL);
		temp = temp->next;
	}
	return (SUCCESS);
}

/*
 * Analyzes the list of tokens and transforms them into
 * a list of commands and file information.
 * 
 * Arguments:
 * - tokens — list of tokens
 * - minishell — general info structure
 * 
 * Returns:
 * List of commands, each command containing a list of arguments
 * and file information (t_command type).
 */
t_list	*parser(t_list *tokens, t_info *minishell)
{
	t_list	*groups;
	t_list	*commands;

	if (!check_syntax(tokens))
		return (free_tokens_return_null(tokens));
	groups = split_groups(tokens);
	if (!groups)
		return (free_tokens_return_null(tokens));
	if (!expand_groups(groups, minishell))
		return (free_groups_return_null(groups));
	commands = get_commands(groups);
	if (!commands)
		return (free_groups_return_null(groups));
	ft_lstclear(&groups, free_token_list);
	ft_lstiter(commands, print_command);
	ft_lstclear(&commands, free_command);
	return (commands);
}
