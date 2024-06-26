/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:05 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/15 10:47:26 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/* Frees the content of a t_list node
 * 
 *
 * Arguments:
 * - char *content
 *
 * Returns:
 * 
 */
void	del_content(void *content)
{
	t_envp	*envp;

	envp = (t_envp *)content;
	free(envp->key);
	if (envp->value)
		free(envp->value);
	free(content);
	content = NULL;
}

/* Deletes envp node
 * 
 *
 * Arguments:
 * - tmp - temp node
 * - current - current node
 * - t_info *struct
 *
 * Returns:
 * 0 if the variable is found and deleted, 1 otherwise
 */
int	handle_del(t_list *tmp, t_list *current, t_info *minishell)
{
	if (!tmp)
	{
		tmp = current->next;
		ft_lstdelone(current, del_content);
		minishell->envp_list = tmp;
		current = minishell->envp_list;
	}
	else
	{
		if (current && current->next)
			tmp->next = current->next;
		else
			tmp->next = NULL;
		ft_lstdelone(current, del_content);
		current = tmp;
	}
	update_envstr(minishell);
	set_pwds(minishell);
	return (SUCCESS);
}

/* Loops through envp_list and if finds a match it deletes it
 * 
 *
 * Arguments:
 * - env var key
 * - t_info *struct
 *
 * Returns:
 * 0 if the variable is found and deleted, 1 otherwise
 */
int	del_env(char *envp_key, t_info *minishell)
{
	t_list	*tmp;
	t_list	*current;

	current = minishell->envp_list;
	tmp = NULL;
	while (current)
	{
		if (!ft_strcmp(envp_key, ((t_envp *)current->content)->key))
			return (handle_del(tmp, current, minishell));
		tmp = current;
		if (current)
			current = current->next;
	}
	set_pwds(minishell);
	return (FAIL);
}

/*
 * Replicates unset builtin command
 *
 * Arguments:
 * - t_list *args
 * - t_info *struct
 *
 * Returns:
 * 0 if all the var are deleted, 1 otherwise
 */
int	handle_unset(t_list *args, t_info *minishell)
{
	int		status;
	t_list	*current;

	status = SUCCESS;
	if (args->next)
	{
		current = args->next;
		while (current)
		{
			if (!status)
				status = del_env((char *)current->content, minishell);
			else
				del_env((char *)current->content, minishell);
			current = current->next;
		}
	}
	return (status);
}
