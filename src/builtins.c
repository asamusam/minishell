/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:07:33 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/25 10:12:03 by mmughedd         ###   ########.fr       */
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
int handle_echo(t_list *args)
{
	t_list *current;
	bool is_n;

	current = args;
	current = current->next;
	is_n = (ft_strcmp((char *)current->content, "-n") == 0);
	while (current)
	{
		ft_putstr_fd((char *)current->content, 1);
		current = current->next;
	}
	if (!is_n)
		ft_putstr_fd("\n", 1);
	return (0);
}

////////////////////////////////////////////////////

char *get_cdpath(t_list *envp)
{
	char *cdpath;
	t_list *current;

	current = envp;
	cdpath = NULL;
	while (current)
	{
		if (!ft_strcmp((char *)current->content, "CDPATH"))
			cdpath = (char *)current->content;
		current = current->next;
	}
	return (cdpath);
}

int update_pwd(char *dir, t_info *info)
{
	return (0);
}

int handle_cd(t_list *args, t_info *info)
{
	char *dir;
	char *cdpath;
	char *dirpath;
	char *tmp;
	char **cdpaths;
	t_list *current;

	if (args->next && args->next->next)
	{
		print_error("bash: cd: too many arguments\n", 1); // only one or two args  TODO: exit
		exit(1);
	}
	else if (!args->next) // no directory provided
	{
		dir = getenv("HOME");
		if (dir == NULL) // 1. If no directory operand is given and the HOME environment variable is empty or undefined, the default behavior is implementation-defined and no further steps shall be taken.
		{
			print_error("HOME environment variable not set.\n", 1);
			return (1);
		}
		chdir(dir);
		exit(0);
	}
	else
	{
		current = args->next;
		dir = (char *)current->content;
		if (dir[0] == '/') // absolute path TODO: what about'~'?
		{
			if (chdir(dir) == -1)
			{
				print_error("0 - chdir error\n", 1);
				return (1);
			}
			// printf("Changed to %s\n", dir);
			exit(0);
		}
		else
		{
			cdpath = get_cdpath(info->envp_list);
			printf("%s", cdpath);
			if (!cdpath) //(!(cdpath = getenv("CDPATH")))
			{
				if (chdir(dir) == -1)
				{
					print_error("1 -chdir error\n", 1);
					return (1);
				}
				printf("1 - Changed to %s\n", dir);
				exit(0);
			}
			else
			{ //TODO: to test
				cdpaths = ft_split(cdpath, ':');
				while (*cdpaths)
				{
					tmp = ft_strjoin(*cdpaths, "/");
					dirpath = ft_strjoin(tmp, dir);
					free(tmp);
					if (!access(dirpath, F_OK))
					{
						free_split(cdpaths);
						if (chdir(dirpath) == -1)
						{
							print_error("2 - chdir error\n", 1);
							return (1);
						}
						printf("2 - Changed to %s\n", dir);
						exit(0);
					}
					cdpaths++;
				}
				free_split(cdpaths);
			}
			// printf("cdpath: %s\n",cdpath);
		}
	}
	return (0);
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
int handle_pwd(void)
{
	char cwd[PATH_MAX]; // TODO: check path_max

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
int update_env(t_list *env_list, char *key, char *value)
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
			update_env(current, key, value);
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
	t_list	*current;

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

////////////////////////////////////////////////////

/* Frees the content of a t_list node
 * 
 *
 * Arguments:
 * - char *content
 *
 * Returns:
 * 
 */
void del_content(void *content)
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

////////////////////////////////////////////////////

/*
 * Replicates env builtin command
 *
 * Arguments:
 * - t_info *struct
 *
 * Returns:
 * Status
 */
int handle_env(t_info *info)
{
	int i;

	i = 0;
	while ((info->envp)[i])
		ft_putendl_fd((info->envp)[i++], 1);
	return (0);
}

////////////////////////////////////////////////////

int handle_exit(t_list *args)
{
	return (0);
}