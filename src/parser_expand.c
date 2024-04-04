/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:08:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/04 17:15:39 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Finds and appends the environment variable value to
 * the given destination string.
 * 
 * Arguments:
 * - key — pointer to the string beginning with a '$' sign
 * - dst — destination string
 * - minishell — general info structure
 * - index — pointer to the index to increment
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	expand_dollar(char *key, char **dst, t_info *minishell, int *index)
{
	int		i;
	int		j;
	char	*var;

	i = 1;
	if (!key[i])
		return (expand_special(dst, ft_strdup("$"), index, i));
	else if (key[i] == '?')
		return (expand_special(dst, ft_itoa(minishell->exit_code), \
				index, i + 1));
	else if (key[i] == '$')
		return (expand_special(dst, ft_strdup("1"), index, i + 1));
	else
	{
		j = i;
		while (key[j] && key[j] != '$' && !ft_isspace(key[j]))
			j ++;
		var = get_envp_value(key + i, j - i, minishell);
		if (var && !concat_strings(dst, ft_strdup(var)))
			return (FAIL);
		*index += j;
	}
	return (SUCCESS);
}

/*
 * Replaces evironment variables in a token with their values.
 * If a variable does not exist, it gets replaced with an empty string.
 * 
 * Arguments:
 * - token — pointer to a token
 * - minishell — general info structure
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	expand_token(t_token *token, t_info *minishell)
{
	char	*before;
	char	*dollar;
	int		i;

	dollar = ft_strchr(token->value, '$');
	if (token->type == FIELD || !dollar)
		return (SUCCESS);
	before = ft_strdup("");
	if (!before)
		return (FAIL);
	i = 0;
	while (dollar)
	{
		if (!concat_strings(&before, \
			ft_substr(token->value, i, dollar - token->value - i)))
			return (free_and_fail(before));
		i = dollar - token->value;
		if (!expand_dollar(dollar, &before, minishell, &i))
			return (free_and_fail(before));
		dollar = ft_strchr(token->value + i, '$');
	}
	if (!concat_strings(&before, ft_strdup(token->value + i)))
		return (free_and_fail(before));
	change_token_value(token, before);
	return (SUCCESS);
}

/*
 * Merges 'src' into 'dst' by appending the value of
 * 'src' to the value of 'dst'. Then frees the 'src'
 * and deletes it from the list of tokens.
 * 
 * Arguments:
 * - dst — pointer to the destination node
 * - src — pointer to the source node
 * 
 * Returns:
 * The merged node on success and NULL if memory allocation fails.
 */
t_list	*merge_nodes(t_list	*dst, t_list *src)
{
	t_token	*dst_token;
	t_token	*src_token;
	char	*new_str;

	if (!src || !dst)
		return (dst);
	dst_token = (t_token *)dst->content;
	src_token = (t_token *)src->content;
	new_str = ft_strjoin(dst_token->value, src_token->value);
	if (!new_str)
		return (NULL);
	free(dst_token->value);
	dst_token->value = new_str;
	dst->next = src->next;
	ft_lstdelone(src, free_token);
	return (dst);
}

/*
 * Merges the 'tokens' node into the 'merge' node and changes
 * the 'tokens' pointer to the resulting merged structure.
 * 
 * Arguments:
 * - tokens — double pointer to the current node in the token list
 * - token — double pointer to the token contained in the current node 
 * of the token list
 * - merge — pointer to the merging destination node of the token list
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	merge_tokens(t_list **tokens, t_token **token, t_list *merge)
{
	*tokens = merge_nodes(merge, *tokens);
	if (*tokens)
		*token = (t_token *)(*tokens)->content;
	else
		return (FAIL);
	return (SUCCESS);
}

/*
 * Goes through a list of tokens and expands everything
 * that has to be expanded (dollar signs and concatenation).
 * 
 * Arguments:
 * - tokens — pointer to the token list
 * - minishell — general info structure
 * 
 * Returns:
 * One on success and zero if memory allocation fails.
 */
int	expand(t_list *tokens, t_info *minishell)
{
	int		prev;
	t_list	*merge;
	t_token	*token;

	merge = NULL;
	prev = 0;
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (is_expandable(token->type))
		{
			if (prev != REDIRECT_INSOURCE && !expand_token(token, minishell))
				return (print_error("minishell parser", PERROR));
			if (is_expandable(prev) && !merge_tokens(&tokens, &token, merge))
				return (print_error("minishell parser", PERROR));
			else
				merge = tokens;
		}
		if (!(token->type == SEPARATOR && prev == REDIRECT_INSOURCE))
			prev = token->type;
		tokens = tokens->next;
	}
	return (SUCCESS);
}
