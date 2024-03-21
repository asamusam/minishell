/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:07:33 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/21 14:56:48 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/exec.h"

/*
 * Replicates echo builtin command, checks for -n option
 *
 * Arguments:
 * 
 * 
 * Returns:
 * Status
 */

int	handle_echo(t_list *args)
{
	t_list	*current;
	int		is_n;

	is_n = 0;
	current = args;
	current = current->next;
	if (!ft_strncmp((char *)current->content, "-n", 2)) //TODO:bool
		is_n = 1;
	while (current)
	{
		ft_putstr_fd((char *)current->content, 1);
		current = current->next;
	}
	if (!is_n)
		ft_putstr_fd("\n", 1);
	return(0);
}

////////////////////////////////////////////////////

int handle_cd(t_list *args, t_info *info)
{
	char	*dir;
	t_list	*current;

	current = args;	
	if (args->next->next)
	{
		print_error("bash: cd: too many arguments", 1); // only one or two args  TODO: exit
		return (2);
	}
	dir = find_envp_line("PWD", info->envp);

	return(0);
}

////////////////////////////////////////////////////

/*
 * Replicates pwd builtin command
 *
 * Arguments:
 * - t_list args
 * 
 * Returns:
 * Status
 */

int	handle_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (1);
	}
	return (0);
}

////////////////////////////////////////////////////

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

int	update_env(t_list *env_list, char *key, char *value)
{
	free(env_list->content);
	env_list->content = (void *)(ft_strdup(value));
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

t_envp	*create_envp_node(char *key, char *value)
{
	t_envp	*node;

	node = malloc(sizeof(t_envp));
	if (!node)
		print_error("malloc error\n", 1);
	node->key = key;
	node->value = value;
	return (node);
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

int	check_envs(t_list *env_list, char *key, char *value)
{
	t_list	*current;
	char	*list_key;
	size_t	len;

	current = env_list;
	len = ft_strlen(key);
	while (current)
	{
		list_key = (char *)(current->content);
		if (len == ft_strlen(list_key))
		{
			if (!strncmp(list_key, key, len))
			{
				update_env(current, key, value);
				return (0);
			}
		}
		current = current->next;
	}
	current = create_envp_node(key, value);
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

int	handle_export(t_list *args, t_info *info) // TODO: export no args
{
	t_list	*current;
	char	**keyval;
	char	*key;
	char	*value;

	if (!((args->next)->content))
		return (0); // TODO: unpspecified behaviour with no args
	//TODO: check if t_list args second node will be "name=john" or "name" and third node "john" // no =
	keyval = ft_split((char *)((args->next)->content), '=');
	key = ft_strdup(keyval[0]);
	if (!keyval[1])
		value = NULL;
	else
		value = ft_strdup(keyval[1]);
	current = (info->envp_list);
	if (!current) //if no env set
		current->content = (void *)create_envp_node(key, value);
	else if (check_envs(current, key, value))
	{
		return (0);
	}
	else
	{
		current = ft_lstlast(current);
		current->next = ft_lstnew((void *)(create_envp_node(key, value)));
	}
	free_split(keyval);
	return (0);
}

////////////////////////////////////////////////////

int	find_env(char *env_var, t_list *env_list)
{
	t_list	*tmp;
	t_list	*current;

	current = env_list;
	while (current)
	{
		if (!ft_strncmp(*env_var, (char *)current->content, ft_strlen(env_var)))
		{
			if (!tmp)
			{
				tmp = current->next;
				ft_lstdelone(current);
				env_list = tmp;
			}
			else
			{
				tmp->next = current->next;
				ft_lstdelone(current);
			}
			return (0);
		}
		tmp = current;
		current = current->next;
	}
	return (1);
}

int	handle_unset(t_list *args, t_info *info)
{
	int		status;
	t_list	*current;

	status = 0;
	if ((args->next)->content)
	{
		current = args->next;
		while (current)
		{
			if (!status)
				status = find_env((char *)current->content, info->envp_list);
			else
				find_env((char *)current->content, info->envp_list);
			current = current->next;
		}
	}
	return(status); // TODO: 0 if all replaced
}

////////////////////////////////////////////////////

/*
 * Replicates env builtin command
 *
 * Arguments:
 * - t_info struct
 * 
 * Returns:
 * Status
 */

int	handle_env(t_info *info)
{
	int	i;

	i = 0;
	while ((info->envp)[i])
		ft_putendl_fd((info->envp)[i++], 1);
	return (0);
}

////////////////////////////////////////////////////

int	handle_exit(t_list *args)
{
	return(0);
}