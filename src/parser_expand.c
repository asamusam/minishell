/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:08:43 by asamuilk          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/20 21:01:12 by asamuilk         ###   ########.fr       */
=======
/*   Updated: 2024/03/22 12:11:51 by asamuilk         ###   ########.fr       */
>>>>>>> 19f3307 (added comments for some functions in parser)
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Checks if the given token type is expandable, i.e.
 * belongs to one of these types: WORD, FIELD, or EXP_FIELD.
 * 
 * Arguments:
 * - type — token type
 * 
 * Returns:
 * One if the given type is expandable and zero if not.
 */
int	is_expandable(int type)
{
	if (type == WORD || type == FIELD || type == EXP_FIELD)
		return (1);
	return (0);
}

char	*get_envp_value(char *key, int len, t_info *minishell)
{
	t_list	*vars;
	t_envp	*var;

	vars = minishell->envp_list;
	while (vars)
	{
		var = (t_envp *)vars->content;
		if (ft_strncmp(key, var->key, len))
			return (var->value);
		vars = vars->next;
	}
	return (NULL);
}

int	expand_token(t_token *token, t_info *minishell)
{
	char	*str;
	char	*join;
	char	*temp;
	int		j;
	int		i;

	if (token->type == FIELD || !ft_strchr(token->value, '$'))
		return (token);
	str = "";
	j = 0;
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$')
		{
			if (!token->value[i + 1])
				break ;
			else if (token->value[i + 1] == '?')
			{
				if (*str)
				{
					temp = ft_itoa(minishell->return_code);
					join = ft_strjoin(str, temp);
					free(str);
					free(temp);
					str = join;
				}
				else
					str = ft_itoa(minishell->return_code);
				i ++;
			}
			else
			{
				j = i + 1;
				while (token->value[j] && token->value[j] != '$' && !ft_isspace(token->value[j]))
					j ++;
				temp = get_envp_value(token->value + i + 1, j - i + 1, minishell);
				if (temp)
				{
					if (*str)
					{
						join = ft_strjoin(str, temp);
						free(str);
						str = join;
					}
					else
						str = ft_strdup(temp);
					i = j - 1;
				}
			}
		}
		i ++;
	}
	if (*str)
	{
		temp = ft_substr(token->value, j, i - j);
		join = ft_strjoin(str, temp);
		free(str);
		free(temp);
		str = join;
	}
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
