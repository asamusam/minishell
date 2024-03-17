/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:07:33 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/17 15:14:32 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"


int	handle_echo(t_command *command)
{
	
}

////////////////////////////////////////////////////

int handle_cd(t_command *command)
{
	
}

////////////////////////////////////////////////////

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

int	update_env(t_list *env_list, char *key, char *value)
{
	free(env_list->content);
	env_list->content = (void *)(ft_strdup(value));
	return (0);
}

int	check_envs(t_list *env_list, char *key, char *value)
{
	t_list	*current;
	char	*list_key;
	size_t		len;

	current = env_list;
	len = ft_strlen(key);
	while (current)
	{
		list_key = (char *)(current->content);
		if (len == ft_strlen(list_key))
		{
			if (!strncmp(list_key, key, len)) //TODO: implement strcmp instead
			{
				update_env(current, key, value);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

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

int	handle_export(t_list *args, t_info *info)
{
	t_list	*current;
	char	**keyval;
	char	*key;
	char	*value;

	//TODO: check if t_list args second node will be "name=john" or "name" and third node "john"
	keyval = ft_split((char *)((args->next)->content), '=');
	key = ft_strdup(keyval[0]);
	value = ft_strdup(keyval[1]);
	current = &(info->envp_list);
	if (!current) //if no env set
		current->content = (void *)create_env(key, value);
	else if (check_envs(current, key, value))
	{
		free_split(keyval);
		return (0);
	}
	else
	{
		current = ft_lstlast(current);
		current->next = create_envp_node(key, value);
	}
	free_split(keyval);
	return (0);
}

////////////////////////////////////////////////////

int	handle_unset(t_command *command)
{
	
}

////////////////////////////////////////////////////

int	handle_env(t_info *info)
{
	int	i;

	i = 0;
	while ((info->envp)[i])
		ft_putendl_fd((info->envp)[i++], 1);
	return (1);
}

////////////////////////////////////////////////////

int	handle_exit(t_command *command)
{
	
}