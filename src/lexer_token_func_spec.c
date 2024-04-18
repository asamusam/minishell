/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_func_spec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:21:27 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/17 16:45:11 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
 * Adds token of type PIPE to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_pipe(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(PIPE, line, len);
	if (!token)
	{
		print_error("Failed to allocate a token", PERROR);
		return (-1);
	}
	node = add_token(token_lst, token);
	if (!node)
	{
		print_error("Failed to allocate a token node", PERROR);
		return (-1);
	}
	return (len);
}

/*
 * Adds token of type REDIRECT_INSOURCE to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_redir_insource(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 2;
	token = create_token(REDIRECT_INSOURCE, line, len);
	if (!token)
	{
		print_error("Failed to allocate a token", PERROR);
		return (-1);
	}
	node = add_token(token_lst, token);
	if (!node)
	{
		print_error("Failed to allocate a token node", PERROR);
		return (-1);
	}
	return (len);
}

/*
 * Adds token of type REDIRECT_IN to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_redir_in(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(REDIRECT_IN, line, len);
	if (!token)
	{
		print_error("Failed to allocate a token", PERROR);
		return (-1);
	}
	node = add_token(token_lst, token);
	if (!node)
	{
		print_error("Failed to allocate a token node", PERROR);
		return (-1);
	}
	return (len);
}

/*
 * Adds token of type REDIRECT_APPEND to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_redir_append(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 2;
	token = create_token(REDIRECT_APPEND, line, len);
	if (!token)
	{
		print_error("Failed to allocate a token", PERROR);
		return (-1);
	}
	node = add_token(token_lst, token);
	if (!node)
	{
		print_error("Failed to allocate a token node", PERROR);
		return (-1);
	}
	return (len);
}

/*
 * Adds token of type REDIRECT_OUT to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_redir_out(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(REDIRECT_OUT, line, len);
	if (!token)
	{
		print_error("Failed to allocate a token", PERROR);
		return (-1);
	}
	node = add_token(token_lst, token);
	if (!node)
	{
		print_error("Failed to allocate a token node", PERROR);
		return (-1);
	}
	return (len);
}
