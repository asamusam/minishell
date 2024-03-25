/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:08:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/25 16:20:11 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	expand_dollar(char	*key, char **dst, t_info *minishell)
{
	int		i;
	int		j;
	char	*join;
	char	*var;

	i = 1;
	if (!key[i])
	{
		join = ft_strdup("$");
		if (!join)
			return (FAIL);
		if (!concat_strings(dst, join))
		{
			free(join);
			return (FAIL);
		}
		return (i);
	}
	if (key[i] == '?')
	{
		join = ft_itoa(minishell->return_code);
		if (!join)
			return (FAIL);
		if (!concat_strings(dst, join))
		{
			free(join);
			return (FAIL);
		}
		return (i + 1);
	}
	if (key[i] == '$')
	{
		join = ft_strdup("1");
		if (!join)
			return (FAIL);
		if (!concat_strings(dst, join))
		{
			free(join);
			return (FAIL);
		}
		return (i + 1);
	}
	j = i;
	while (key[j] && key[j] != '$' && !ft_isspace(key[j]))
		j ++;
	var = get_envp_value(key + i, j - i, minishell);
	if (var)
	{
		join = ft_strdup(var);
		if (!join)
			return (FAIL);
		if (!concat_strings(dst, join))
		{
			free(join);
			return (FAIL);
		}
	}
	return (j);
}

int	expand_token(t_token *token, t_info *minishell)
{
	char	*str;
	char	*before;
	char	*dollar;
	char	*temp;
	int		i;
	int		add;

	str = token->value;
	dollar = ft_strchr(str, '$');
	if (token->type == FIELD || !dollar)
		return (SUCCESS);
	before = ft_substr(str, 0, dollar - str);
	if (!before)
		return (FAIL);
	i = dollar - str;
	while (dollar)
	{
		add = expand_dollar(dollar, &before, minishell);
		if (!add)
		{
			free(before);
			return (FAIL);
		}
		i += add;
		dollar = ft_strchr(str + i, '$');
		if (dollar)
		{
			temp = ft_substr(str, i, dollar - str - i);
			if (!temp)
			{
				free(before);
				return (FAIL);
			}
			if (!concat_strings(&before, temp))
			{
				free(before);
				free(temp);
				return (FAIL);
			}
			i = dollar - str;
		}
	}
	temp = ft_strdup(str + i);
	if (!temp || !concat_strings(&before, temp))
	{
		free(before);
		free(temp);
		return (FAIL);
	}
	free(token->value);
	token->value = before;
	return (SUCCESS);
}

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
			if (is_expandable(prev))
			{
				tokens = merge_nodes(merge, tokens);
				if (tokens)
					token = (t_token *)tokens->content;
				else
					return (print_error("minishell parser", PERROR));
			}
			else
				merge = tokens;
		}
		if (!(token->type == SEPARATOR && prev == REDIRECT_INSOURCE))
			prev = token->type;
		tokens = tokens->next;
	}
	return (SUCCESS);
}
