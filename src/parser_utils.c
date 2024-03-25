/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:03:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/25 16:02:05 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * Prints the contents of the group of tokens.
 *
 * Arguments:
 * - arg — a node in the list of groups.
 * 
 * Returns:
 * Nothing.
 */
void	print_group(void *arg)
{
	t_list	*group;

	group = (t_list *)arg;
	printf("----GROUP-----\n");
	ft_lstiter(group, print_token);
}

/*
 * Frees all the memory associated with the list of tokens.
 * 
 * Arguments:
 * - arg — pointer to the token list
 * 
 * Returns:
 * Nothing.
 */
void	free_token_list(void *arg)
{
	t_list	*tokens;

	tokens = (t_list *)arg;
	ft_lstclear(&tokens, free_token);
}

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

/*
 * Looks up the environment variable value by the given key.
 * 
 * Arguments:
 * - key — pointer to the beginning of the key to look for
 * - len — the length of the key
 * - minishell — general info structure
 * 
 * Returns:
 * Environment variable value — pointer to the string
 * contained in the var->value in the envp list.
 * NULL if the key not found.
 */
char	*get_envp_value(char *key, int len, t_info *minishell)
{
	t_list	*vars;
	t_envp	*var;

	if (!len)
		return (NULL);
	vars = minishell->envp_list;
	while (vars)
	{
		var = (t_envp *)vars->content;
		if (!ft_strncmp(key, var->key, len) && !var->key[len])
			return (var->value);
		vars = vars->next;
	}
	return (NULL);
}

/*
 * Joins two strings, assigns the resulting value to str, 
 * and frees the original strings.
 * 
 * Arguments:
 * - str — the destination string
 * - to_join — the string to append to str
 * 
 * Returns:
 * One on success and zero if the memory allocation fails.
 */
int	concat_strings(char **str, char *to_join)
{
	char	*joined;

	if (!to_join)
		return (SUCCESS);
	if (!*to_join)
	{
		free(to_join);
		return (SUCCESS);
	}
	if (!*str)
		*str = to_join;
	else
	{
		joined = ft_strjoin(*str, to_join);
		if (!joined)
			return (FAIL);
		free(*str);
		free(to_join);
		*str = joined;
	}
	return (SUCCESS);
}
