/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:50 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 12:04:40 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
int	check_envs(t_info *minishell, char *key, char *value)
{
	t_list	*current;
	char	*list_key;

	current = minishell->envp_list;
	while (current)
	{
		list_key = ((t_envp *)current->content)->key;
		if (!ft_strcmp(list_key, key))
		{
			update_envp(current, value);
			return (SUCCESS);
		}
		current = current->next;
	}
	ft_lstadd_back(&minishell->envp_list, create_envp_node(key, value));
	return (SUCCESS);
}

void	handle_key_val(t_list *args, char **key, char **value)
{
	if (find_equal((char *)(args->next)->content) == -1)
	{
		*key = ft_strdup((char *)(args->next)->content);
		*value = NULL;
	}
	else
		get_keyval((char *)(args->next)->content, key, value);
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
	char	*key;
	char	*value;

	if (!args->next)
		return (print_export(info));
	if (args->next && args->next->next)
		return (0);
	if (!check_input((char *)(args->next)->content))
		return (print_error(EXPORT_ERROR, STDERR));
	handle_key_val(args, &key, &value);
	if (!key)
		return (FAIL);
	if (!info->envp_list)
		info->envp_list = create_envp_node(key, value);
	else
		check_envs(info, key, value);
	update_envstr(info);
	set_pwds(info);
	free(key);
	if (value)
		free(value);
	return (SUCCESS);
}
