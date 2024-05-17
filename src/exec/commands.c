/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 00:11:32 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/22 23:12:00 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
char	*get_cmd(char **path, char *command)
{
	char	*tmp;
	char	*cmd;
	int		i;

	if (ft_strchr(command, '/'))
	{
		if (!access(command, 0))
			return (command);
		else
			return (NULL);
	}
	else if (path && *path)
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
	if (!args)
	{
		print_error(MALLOC_ERROR, PERROR);
		return (NULL);
	}
	while (i < len && current)
	{
		args[i++] = ft_strdup((char *)(current->content));
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

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
int	handle_input(t_command *command, t_info *minishell)
{
	char	**args;
	char	*cmd;

	args = NULL;
	cmd = NULL;
	args = get_args(command->args);
	if (args)
		cmd = get_cmd(minishell->path, args[0]);
	if (!cmd)
	{
		print_error(COMMAND_ERROR, STDERR);
		free_split(args);
		return (127);
	}
	if (execve(cmd, args, minishell->envp) == -1)
	{
		free_split(args);
		print_error(EXECVE_ERROR, PERROR);
		return (126);
	}
	return (SUCCESS);
}

/*
 * Checks if the string provided is a built-in cmd
 *
 * Arguments:
 * - the command string to check
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */
int	is_builtin(char *cmd)
{
	if (!ft_strcmp("echo", cmd))
		return (1);
	else if (!ft_strcmp("cd", cmd))
		return (2);
	else if (!ft_strcmp("pwd", cmd))
		return (3);
	else if (!ft_strcmp("export", cmd))
		return (4);
	else if (!ft_strcmp("unset", cmd))
		return (5);
	else if (!ft_strcmp("env", cmd))
		return (6);
	else if (!ft_strcmp("exit", cmd))
		return (7);
	return (0);
}

/*
 * Calls the handler function depending on the built-in command
 *
 * Arguments:
 * - command struct
 * - t_info struct
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */
int	handle_builtin(t_command *command, t_info *minishell)
{
	int		n;
	t_list	*args;

	args = command->args;
	n = is_builtin(args->content);
	if (n == 1)
		return (handle_echo(args));
	else if (n == 2)
		return (handle_cd(args, minishell));
	else if (n == 3)
		return (handle_pwd(minishell));
	else if (n == 4)
		return (handle_export(args, minishell));
	else if (n == 5)
		return (handle_unset(args, minishell));
	else if (n == 6)
		return (handle_env(minishell));
	else if (n == 7)
		return (handle_exit(args, minishell));
	return (0);
}
