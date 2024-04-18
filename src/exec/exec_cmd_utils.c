/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:36:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/18 01:11:19 by asamuilk         ###   ########.fr       */
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

	if (strchr(command, '/'))
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
		print_error(COMMAND_ERROR, PERROR);
		free_split(args);
		return (126);
	}
	if (execve(cmd, args, minishell->envp) == -1)
	{
		free_split(args);
		print_error(EXECVE_ERROR, PERROR);
		return (127);
	}
	return (SUCCESS);
}

int	handle_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell)
{
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error(FORK_ERROR, PERROR));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redirections(pipet, command) == FAIL)
			exit(FAIL);
		exit(handle_input(command, minishell));
	}
	else
		return (handle_parent(pipet));
}

int	handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell)
{
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error(FORK_ERROR, PERROR));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_last_redirection(pipet, command) == FAIL)
			exit(FAIL);
		exit(handle_input(command, minishell));
	}
	else
		return (handle_last_parent(pipet));
}
