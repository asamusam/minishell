/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:50 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/07 09:17:01 by mmughedd         ###   ########.fr       */
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
int	update_envp(t_list *envp_list, char *value)
{
	free(((t_envp *)envp_list->content)->value);
	if (value)
		((t_envp *)envp_list->content)->value = ft_strdup(value);
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
t_list	*create_envp_node(char *key, char *value)
{
	t_envp	*node;

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
 * Updates or create env var
 *
 * Arguments:
 * - env var key
 * - env var value
 *
 * Returns:
 * Status
 */
int	check_envs(t_info *info, char *key, char *value)
{
	t_list	*current;
	char	*list_key;

	current = info->envp_list;
	while (current)
	{
		list_key = ((t_envp *)current->content)->key;
		if (!ft_strcmp(list_key, key))
		{
			update_envp(current, value);
			return (0);
		}
		current = current->next;
	}
	ft_lstadd_back(&info->envp_list, create_envp_node(key, value));
	return (0);
}

/*
 * Handles export builtin command
 *
 * Arguments:
 * - args - variable attributes
 * - info - info sturcture
 *
 * Returns:
 * Status
 */
int	handle_export(t_list *args, t_info *info)
{
	char	**keyval;
	char	*key;
	char	*value;

	if (!args->next)
		return (print_export(info));
	if (!check_input((char *)(args->next)->content))
		return (print_error("bash: export: not a valid identifier\n", 1));
	if (info->is_multiple_proc)
		return (0);
	if (find_equal((char *)(args->next)->content) == -1)
	{
		key = ft_strdup((char *)(args->next)->content);
		value = NULL;
	}
	else
		get_keyval((char *)(args->next)->content, &key, &value);
	if (!info->envp_list)
		info->envp_list = create_envp_node(key, value);
	else
		check_envs(info, key, value);
	update_envstr(info);
	free(key);
	if (value)
		free(value);
	return (0);
}
