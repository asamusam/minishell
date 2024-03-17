/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:36:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/17 14:04:32 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
	char	cmd;

	get_args(command->args ,args);
	cmd = get_command(info->path, args[0]);
	if (!cmd)
	{
		print_error("Invalid cmd\n", 0); // TODO: get correct error
		free_split(args);
		return (1);
	}
	execve(cmd, args, info->envp);
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

void	get_args(t_list *arg_lst, char **args)
{
	int		len;
	int		i;
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
	args[i] = '/0';
}

