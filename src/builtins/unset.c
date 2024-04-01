/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:26:05 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/30 13:26:17 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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
	free(content);
	content = NULL;
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
int del_env(char *envp_key, t_info *info)
{
	t_list	*tmp;
	t_list	*current;

	current = info->envp_list;
	tmp = NULL;
	while (current)
	{
		if (!ft_strcmp(envp_key, ((t_envp *)current->content)->key))
		{
			if (!tmp)
			{
				tmp = current->next;
				ft_lstdelone(current, del_content);
				info->envp_list = tmp;
				current = info->envp_list;
			}
			else
			{
				tmp->next = current->next;
				ft_lstdelone(current, del_content);
				current = tmp;
			}
			return (0);
		}
		tmp = current;
		if (current)
			current = current->next;
	}
	return (1);
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
int handle_unset(t_list *args, t_info *info)
{
	int status;
	t_list *current;

	status = 0;
	if (args->next)
	{
		current = args->next;
		while (current)
		{
			if (!status)
				status = del_env((char *)current->content, info);
			else
				del_env((char *)current->content, info);
			current = current->next;
		}
	}
	return (status); // TODO: 0 if all replaced
}