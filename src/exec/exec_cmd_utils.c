/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:36:09 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/16 15:16:42 by mmughedd         ###   ########.fr       */
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

	if (!command)
		return (FAIL); //TODO:
	args = get_args(command->args);
	cmd = get_cmd(minishell->path, args[0]);
	if (!cmd)
	{
		print_error("Cmd error", 0);
		free_split(args);
		return (127);
	}
	if (execve(cmd, args, minishell->envp) == -1)
	{
		free_split(args);
		print_error("Execve error", 0);
		return (126);
	}
	return (FAIL);
}

int	handle_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell)
{
	int		child_status;
	void	(*parent_handler)(int);

	if (!command)
		return (FAIL); //TODO:
	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_redirections(pipet, command);
		exit(handle_input(command, minishell));
	}
	else
	{
		parent_handler = signal(SIGINT, SIG_IGN);
		close(pipet->pipefd[1]);
		close(pipet->prev_pipe);
		pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, &child_status, 0);
		signal(SIGINT, parent_handler);
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		else
		{
			if (WTERMSIG(child_status) == SIGINT)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (WTERMSIG(child_status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
			return (128 + WTERMSIG(child_status));
		}
	}
}



int	handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell)
{
	int		child_status;
	void	(*parent_handler)(int);

	pipet->pid = fork();
	if (pipet->pid == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_last_redirection(pipet, command);
		exit(handle_input(command, minishell));
	}
	else
	{
		parent_handler = signal(SIGINT, SIG_IGN);
		close(pipet->prev_pipe);
		waitpid(pipet->pid, &child_status, 0);
		signal(SIGINT, parent_handler);
		if (WIFEXITED(child_status))
			return (WEXITSTATUS(child_status));
		else
		{
			if (WTERMSIG(child_status) == SIGINT)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (WTERMSIG(child_status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
			return (128 + WTERMSIG(child_status));
		}
	}
}
