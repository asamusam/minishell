/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:50 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/28 11:29:19 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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

	node = malloc(sizeof(t_envp));
	if (!node)
		print_error("malloc error\n", 1);
	node->key = key;
	node->value = value;
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
	t_list *current;
	char *list_key;

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
int handle_export(t_list *args, t_info *info) // TODO: export no args
{
	char	**keyval;
	char	*key;
	char	*value;

	if ((args->next) && !((args->next)->content))
		return (0); // TODO: unpspecified behaviour with no args
	// TODO: check if t_list args second node will be "name=john" or "name" and third node "john" // no =
	keyval = ft_split((char *)((args->next)->content), '=');
	key = ft_strdup(keyval[0]);
	if (!keyval[1])
		value = NULL; //TODO: check if ""
	else
		value = ft_strdup(keyval[1]);
	if (!info->envp_list) // if no env set
		info->envp_list = create_envp_node(key, value);
	else
		check_envs(info, key, value);
	free_split(keyval);
	return (0);
}