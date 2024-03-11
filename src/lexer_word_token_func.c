/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_token_func.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:21:27 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/11 19:53:29 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "fcntl.h"

int	add_word(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 0;
	while (is_word(line[len]))
		len ++;
	token = create_token(WORD, line, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_separator(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;

	len = 0;
	while (ft_isspace(line[len]))
		len ++;
	token = create_token(SEPARATOR, NULL, 1);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len);
}

int	add_exp_field(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;
	int		i;

	len = 0;
	i = 1;
	while (line[i] && line[i] != '\"')
	{
		len ++;
		i ++;
	}
	if (!line[i])
	{
		ft_putendl_fd("Syntax error: unclosed double quotes", STDERR_FILENO);
		return (-1);
	}
	if (len)
		token = create_token(EXP_FIELD, line + 1, len);
	else
		token = create_token(EXP_FIELD, NULL, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len + 2);
}

int	add_field(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		len;
	int		i;

	len = 0;
	i = 1;
	while (line[i] && line[i] != '\'')
	{
		len ++;
		i ++;
	}
	if (!line[i])
	{
		ft_putendl_fd("Syntax error: unclosed single quotes", STDERR_FILENO);
		return (-1);
	}
	if (len)
		token = create_token(FIELD, line + 1, len);
	else
		token = create_token(FIELD, NULL, len);
	if (!token)
		exit(EXIT_FAILURE);
	node = add_token(token_lst, token);
	if (!node)
		exit(EXIT_FAILURE);
	return (len + 2);
}
