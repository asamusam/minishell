/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:25:50 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/19 09:34:21 by mmughedd         ###   ########.fr       */
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

int	handle_key_val(t_list *args, char **key, char **value)
{
	if (find_equal((char *)args->content) == -1)
		return (-1);
	else
		return (get_keyval((char *)args->content, key, value));
}

void	export_env(t_info *minishell, char **key, char **value)
{
	if (!key)
		return ;
	if (!minishell->envp_list)
		minishell->envp_list = create_envp_node(*key, *value);
	else
		check_envs(minishell, *key, *value);
	update_envstr(minishell);
	set_pwds(minishell);
	free(*key);
	if (*value)
		free(*value);
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
int	handle_export(t_list *args, t_info *minishell)
{
	char	*key;
	char	*value;
	t_list	*current;

	if (!args->next)
		return (print_export(minishell));
	current = args->next;
	while (current)
	{
		if (!check_input((char *)current->content))
			return (print_error(EXPORT_ERROR, STDERR));
		if (handle_key_val(current, &key, &value) == -1)
		{
			current = current->next;
			continue ;
		}
		export_env(minishell, &key, &value);
		current = current->next;
	}
	return (SUCCESS);
}
