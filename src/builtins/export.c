/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:50 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/31 15:25:58 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
 * Updates a env var value in the env_list list
 *
 * Arguments:
 * - t_list env_list
 * - env var key
 * - env var value
 *
 * Returns:
 * Status
 */
int update_env(t_list *env_list, char *value)
{
	//free(((t_envp *)env_list->content)->value); TODO: check
	if (value)
		((t_envp *)env_list->content)->value = ft_strdup(value);
	return (0);
}

/*
 * Creates an envp_node in the t_envp struct
 *
 * Arguments:
 * - env var key
 * - env var value
 *
 * Returns:
 * Status
 */
t_list *create_envp_node(char *key, char *value)
{
	t_envp *node;

	if (!key)
		return (NULL);
	node = malloc(sizeof(t_envp));
	if (!node)
		print_error("malloc error\n", 1);
	node->key = ft_strdup(key);
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	return (ft_lstnew((void *)node));
}

/*
 * Checks if there's already an env var with the same key. If there's it updates it, otherwise it creates a new list node with those values
 *
 * Arguments:
 * - env var key
 * - env var value
 *
 * Returns:
 * Status
 */
int check_envs(t_info *info, char *key, char *value)
{
	t_list	*current;
	char	*list_key;

	current = info->envp_list;
	while (current)
	{
		list_key = ((t_envp *)current->content)->key;
		if (!ft_strcmp(list_key, key))
		{
			update_env(current, value);
			return (0);
		}
		current = current->next;
	}
	ft_lstadd_back(&info->envp_list, create_envp_node(key, value));
	return (0);
}

int	find_equal(char *input)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(input);
	while (input[i] && i < len)
	{
		if (input[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}


void	get_keyval(char *input, char **key, char **value)
{
	int	equal_len;
	int	len;

	equal_len = find_equal(input);
	len = ft_strlen(input);
	if (equal_len == len - 1)
	{
		*key = ft_substr(input, 0, len - 1);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(input, 0, equal_len);
		*value = ft_substr(input, equal_len + 1, len - equal_len);
	}
}

int	check_input(char *input)
{
	int	i;
	int	equal_len;

	i = 0;
	equal_len = find_equal(input);
	if (equal_len == -1)
		equal_len = ft_strlen(input);
	if (equal_len == 0 || (!ft_isalpha(input[i]) && input[i] != '_'))
		return (0);
	while (input[++i] && i < equal_len)
	{
		if (!ft_isalnum(input[i]) && input[i++] != '_')
			return (0);
	}
	return (1);
}

/*
 * Handles export builtin command
 *
 * Arguments:
 * - t_list args
 * - t_info info
 *
 * Returns:
 * Status
 */
int handle_export(t_list *args, t_info *info)
{
	char	*input;
	char	**keyval;
	char	*key;
	char	*value;

	if (!args->next)
	{
		return (0); // TODO: print all envp, to determine how to store them
	}
	input = (char *)(args->next)->content;
	if (!check_input(input))
	{
		print_error("bash: export: not a valid identifier\n", 1);
		return (0);
	}
	if (find_equal(input) == -1)
	{
		key = ft_strdup(input);
		value = NULL;
	}
	else
		get_keyval(input, &key, &value);
	if (!info->envp_list) // if no env set
		info->envp_list = create_envp_node(key, value);
	else
		check_envs(info, key, value);
	free(key);
	if (value)
		free(value);
	return (0);
}