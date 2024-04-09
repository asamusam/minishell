/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:36:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/08 13:06:23 by mmughedd         ###   ########.fr       */
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

	if (!command)
		return (1);
	args = get_args(command->args);
	cmd = get_cmd(info->path, args[0]);
	if (!cmd)
	{
		print_error("Cmd error\n", 0);
		free_split(args);
		return (127);
	}
	chdir(info->pwd);
	if (execve(cmd, args, info->envp) == -1)
	{
		free_split(args);
		return (print_error("Execve error\n", 0));
	}
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

int	handle_cmd_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int	status;

	if (!command)
		return (1);
	status = 0;
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		handle_redirections(pipet, command);
		status = handle_input(command, info);
	}
	else
	{
		close(pipet->pipefd[1]);
		close(pipet->prev_pipe);
		pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, NULL, 0);
	}
	return (status);
}

int	handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int		status;

	if (!command)
		return (1);
	status = 0;
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		handle_last_redirection(pipet, command);
		status = handle_input(command, info);
	}
	else
	{
		close(pipet->prev_pipe);
		waitpid(pipet->pid, NULL, 0);
	}
	return (status);
}
