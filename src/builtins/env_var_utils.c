/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:43:05 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 14:05:38 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	**copy_envp(char **envp)
{
	int		i;
	char	**envp_copy;

	if (!*envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	envp_copy = malloc(sizeof(char *) * (i + 1));
	if (!envp_copy)
	{
		print_error(MALLOC_ERROR, PERROR);
		return (NULL);
	}
	envp_copy[i] = NULL;
	i = -1;
	while (envp[++i])
		envp_copy[i] = ft_strdup(envp[i]);
	return (envp_copy);
}

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
	else
		((t_envp *)envp_list->content)->value = NULL;
	return (SUCCESS);
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
		print_error(MALLOC_ERROR, PERROR);
	node->key = ft_strdup(key);
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	return (ft_lstnew((void *)node));
}

void	replace_pwd(t_list **envp_list, char *newpwd, t_info *minishell)
{
	free(((t_envp *)(*envp_list)->content)->value);
	((t_envp *)(*envp_list)->content)->value = ft_strdup(newpwd);
	minishell->pwd = ((t_envp *)(*envp_list)->content)->value;
}

/*
 * Updates PWD and OLDPWD env variables
 *
 * Arguments:
 * - info - info structure
 * - newpwd - newpwd name variable 
 * cd
 * Returns:
 * 0 on success, 1 on error
 */
int	update_envp_pwd(t_info *minishell, char *newpwd)
{
	char	*oldpwd;
	char	*key;
	t_list	*envp_list;

	envp_list = minishell->envp_list;
	oldpwd = ft_strdup(minishell->pwd);
	if (minishell->oldpwd == NULL)
		ft_lstadd_back(&envp_list, create_envp_node("OLDPWD", NULL));
	while (envp_list)
	{
		key = ((t_envp *)envp_list->content)->key;
		if (key && !ft_strncmp(key, "PWD", 3))
			replace_pwd(&envp_list, newpwd, minishell);
		else if (key && !ft_strncmp(key, "OLDPWD", 6))
		{
			free(((t_envp *)envp_list->content)->value);
			((t_envp *)envp_list->content)->value = oldpwd;
			minishell->oldpwd = ((t_envp *)envp_list->content)->value;
		}
		envp_list = envp_list->next;
	}
	update_envstr(minishell);
	return (SUCCESS);
}
