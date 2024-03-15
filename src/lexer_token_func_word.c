/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_func_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:21:27 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/13 17:01:32 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "fcntl.h"

/*
 * Adds token of type WORD to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_word(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		i;

	i = 1;
	while (is_word(line[i]))
		i ++;
	token = create_token(WORD, line, i);
	if (!token)
		return (print_error("Failed to allocate a token", PERROR));
	node = add_token(token_lst, token);
	if (!node)
		return (print_error("Failed to allocate a token node", PERROR));
	return (i);
}

/*
 * Adds token of type SEPARATOR to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_separator(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		i;

	i = 1;
	while (ft_isspace(line[i]))
		i ++;
	token = create_token(SEPARATOR, " ", 1);
	if (!token)
		return (print_error("Failed to allocate a token", PERROR));
	node = add_token(token_lst, token);
	if (!node)
		return (print_error("Failed to allocate a token node", PERROR));
	return (i);
}

/*
 * Adds token of type EXP_FIELD to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_exp_field(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		i;

	i = 1;
	while (line[i] && line[i] != '\"')
		i ++;
	if (!line[i])
		return (print_error("Syntax error: unclosed double quotes", STDERR));
	if (i - 1)
		token = create_token(EXP_FIELD, line + 1, i - 1);
	else
		token = create_token(EXP_FIELD, "", i - 1);
	if (!token)
		return (print_error("Failed to allocate a token", PERROR));
	node = add_token(token_lst, token);
	if (!node)
		return (print_error("Failed to allocate a token node", PERROR));
	return (i + 1);
}

/*
 * Adds token of type FIELD to the token list.
 *
 * Arguments:
 * - token_lst — list to add a token to
 * - line — pointer to the beginning of a token in the user input string
 * 
 * Returns:
 * The length of the token, including quotes where applicable.
 */
int	add_field(t_list **token_lst, char *line)
{
	t_token	*token;
	t_list	*node;
	int		i;

	i = 1;
	while (line[i] && line[i] != '\'')
		i ++;
	if (!line[i])
		return (print_error("Syntax error: unclosed single quotes", STDERR));
	if (i - 1)
		token = create_token(FIELD, line + 1, i - 1);
	else
		token = create_token(FIELD, "", i - 1);
	if (!token)
		return (print_error("Failed to allocate a token", PERROR));
	node = add_token(token_lst, token);
	if (!node)
		return (print_error("Failed to allocate a token node", PERROR));
	return (i + 1);
}
