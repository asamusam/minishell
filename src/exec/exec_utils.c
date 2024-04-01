/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:36:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/01 14:17:02 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
 * Divides the input in cmd and args and exec them
 *
 * Arguments:
 * - input
 * - info struct
 * 
 * Returns:
 * Status
 */
int	handle_input(t_command *command, t_info *info)
{
	char	**args;
	char	*cmd;

	args = get_args(command->args);
	cmd = get_command(info->path, args[0]);
	if (!cmd)
	{
		print_error("cmd error", 0); // TODO: get correct error
		free_split(args);
		return (127); //command not found status
	}
	execve(cmd, args, info->envp);
	print_error("execve error", 1);
	return (0);
}

/*
 * Checks if the string provided is a valid cmd
 *
 * Arguments:
 * - path
 * - command
 * 
 * Returns:
 * The cmd string if valid, otherwise NULL
 */
char	*get_command(char **path, char *command)
{
	char	*tmp;
	char	*cmd;
	int		i;

	if (strchr(command, '/')) // TODO: confirm that's correct or simply check for command[0] == '/'
	{
		if (!access(command, 0))
			return (command);
		else
			return (NULL);
	}
	else
	{
		i = 0;
		while (path[i])
		{
			tmp = ft_strjoin(path[i++], "/");
			cmd = ft_strjoin(tmp, command);
			free(tmp);
			if (!access(cmd, 0))
				return (cmd);
			free(cmd);
		}
	}
	return (NULL);
}

/*
 * From list allocates and fills array of arguments (necessary for execv)
 *
 * Arguments:
 * - list of arguments
 * - array of arguments
 * 
 * Returns:
 * void
 */
char	**get_args(t_list *arg_lst)
{
	int		len;
	int		i;
	char	**args;
	t_list	*current;

	len = ft_lstsize(arg_lst);
	i = 0;
	current = arg_lst;
	args = malloc(sizeof(char *) * (len + 1));
	while (i < len && current)
	{
		args[i++] = ft_strdup((char *)(current->content));
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void del_env_content(void *content)
{
	t_envp	*envp;
	
	envp = (t_envp *)content;
	if (!envp)
		return ;
	free(envp->key);
	free(envp->value);
	free(content);
}

void	t_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*node;
	t_list	*next_node;

	if (!*lst)
		return ;
	node = *lst;
	while (node)
	{
		del(node->content);
		next_node = node->next;
		free(node);
		node = next_node;
	}
	*lst = NULL;
}

void	free_info(t_info *info)
{
	free_split(info->path);
	free_split(info->envp);
	t_lstclear(&(info->envp_list), &del_env_content);
	free(info);
}