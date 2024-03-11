/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_spec_token_func.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:21:27 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/11 16:54:23 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	add_pipe(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(PIPE, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_redir_insource(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 2;
	token = create_token(REDIRECT_INSOURCE, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_redir_in(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(REDIRECT_IN, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_redir_append(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 2;
	token = create_token(REDIRECT_APPEND, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_redir_out(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 1;
	token = create_token(REDIRECT_OUT, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}
