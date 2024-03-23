/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:08:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/23 21:28:34 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	concat_strings(char **str, char *to_join)
{
	char	*joined;

	if (!to_join)
		return ;
	if (!*to_join)
	{
		free(to_join);
		return ;
	}
	if (!*str)
		*str = to_join;
	else
	{
		joined = ft_strjoin(*str, to_join);
		free(*str);
		free(to_join);
		*str = joined;
	}
}

int	expand_dollar(char	*key, char **to_join, t_info *minishell)
{
	int	i;
	int	j;

	i = 1;
	if (!key[i])
	{
		concat_strings(to_join, ft_strdup("$"));
		return (1);
	}
	if (key[i] == '?')
	{
		concat_strings(to_join, ft_itoa(minishell->return_code));
		return (2);
	}
	if (key[i] == '$')
	{
		concat_strings(to_join, ft_strdup("1"));
		return (2);
	}
	j = i;
	while (key[j] && key[j] != '$' && !ft_isspace(key[j]))
		j ++;
	concat_strings(to_join, get_envp_value(key + i, j - i, minishell));
	return (j);
}

void	expand_token(t_token *token, t_info *minishell)
{
	char	*str;
	char	*before;
	char	*dollar;
	int		i;

	str = token->value;
	dollar = ft_strchr(str, '$');
	if (token->type == FIELD || !dollar)
		return ;
	before = ft_substr(str, 0, dollar - str);
	i = dollar - str;
	while (dollar)
	{
		i += expand_dollar(dollar, &before, minishell);
		dollar = ft_strchr(str + i, '$');
		if (dollar)
		{
			concat_strings(&before, ft_substr(str, i, dollar - str - i));
			i = dollar - str;
		}
	}
	concat_strings(&before, ft_strdup(str + i));
	free(token->value);
	token->value = str;
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
	free(dst_token->value);
	dst_token->value = new_str;
	dst->next = src->next;
	ft_lstdelone(src, free_token);
	return (dst);
}

void	expand(t_list *tokens, t_info *minishell)
{
	int		prev;
	int		prev_no_space;
	t_list	*merge;
	t_token	*token;

	merge = NULL;
	prev = 0;
	prev_no_space = 0;
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (is_expandable(token->type))
		{
			if (prev_no_space != REDIRECT_INSOURCE)
				expand_token(token, minishell);
			if (is_expandable(prev))
			{
				tokens = merge_nodes(merge, tokens);
				token = (t_token *)tokens->content;
			}
			else
				merge = tokens;
		}
		if (token->type != SEPARATOR)
			prev_no_space = token->type;
		prev = token->type;
		tokens = tokens->next;
	}
}
