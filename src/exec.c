/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/15 14:36:10 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Checks if the string provided is a built-in cmd
 *
 * Arguments:
 * - cmd - string
 * 
 * Returns:
 * Number corresponding to cmd, 0 if not built-in
 */

int	is_buit_in(char *cmd)
{
	if (!ft_strncmp("echo", cmd, ft_strlen(cmd)))
		return (1);
	else if (!ft_strncmp("cd", cmd, ft_strlen(cmd)))
		return (2);
	else if (!ft_strncmp("pwd", cmd, ft_strlen(cmd)))
		return (3);
	else if (!ft_strncmp("export", cmd, ft_strlen(cmd)))
		return (4);
	else if (!ft_strncmp("unset", cmd, ft_strlen(cmd)))
		return (5);
	else if (!ft_strncmp("env", cmd, ft_strlen(cmd)))
		return (6);
	else if (!ft_strncmp("exit", cmd, ft_strlen(cmd)))
		return (7);
	return (0);
}

int	handle_built_in(char *input)
{
	int	n;

	n = is_buit_in(input);
	if (n == 1)
		handle_echo(input);
	else if (n == 2)
		handle_cd(input);
	else if (n == 3)
		handle_pwd(input);
	else if (n == 4)
		handle_export(input);
	else if (n == 5)
		handle_unset(input);
	else if (n == 6)
		handle_env(input);
	else if (n == 7)
		handle_exit(input);
	// TODO: handle errors/return
	return(0);
}

/*
 * Checks if the string provided is a valid cmd and if so returns it
 *
 * Arguments:
 * - path - available paths
 * - command - command name
 * 
 * Returns:
 * String to input in execve, otherwise null
 */

char	*get_command(char **path, char *command)
{
	char	*tmp;
	char	*cmd;
	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		cmd = ft_strjoin(tmp, command);
		free(tmp);
		if (!access(cmd, 0))
			return (cmd);
		free(cmd);
		path++;
	}
	return (NULL);
}

/*
 * Divided the input in cmd and args and exec them
 *
 * Arguments:
 * - input
 * - info struct
 * 
 * Returns:
 * 
 */

int	handle_input(char *input, t_info *info)
{
	char	**args;
	char	cmd;

	// if string with no redirection
	args = ft_split(args, ' ');
	cmd = get_command(info->path, args[0]);
	if (!cmd)
	{
		print_error("Invalid cmd\n", 0); // TODO: get correct error
		exit (1);
	}
	execve(cmd, args, info->envp);
	return (0);
}

/*
 * Create processes, pipes and executes commands
 *
 * Arguments:
 * - env - enviromental variables
 * - TBD
 * 
 * Returns:
 * TBD
 */

int	ft_pipe(t_list **processes, t_info *info)
{
	int		n_process;
	char	input[] = "grep Hello"; // depending on struct it'll be the input to analize

	n_process = 3; // function to count number of sublists
	int		pipes[n_process][2];
	pid_t	pid[n_process];
	int		i;

	i = 0;
	while (i < n_process)
	{
		if (pipe(pipes[i]) < 0)
		{
			print_error("Pipe error\n", 0); // TODO: get correct error
			return (1);
		}
	}
	i = 0;
	while (i < n_process)
	{
		if ((pid[i] = fork()) == -1)
			print_error("Fork error\n", 0); // TODO: get correct error, also EXIT?
		if (pid[i] == 0)
		{
			if (i > 0) // not first process
				dup2(pipes[i][0], STDIN_FILENO); // read from pipe
			if (i < n_process)
				dup2(pipes[i][1], STDOUT_FILENO); // write into pipe
			close(pipes[i][0]);
			close(pipes[i][1]);
			if (!is_buit_in(input))
				handle_built_in(input);
			else
				handle_input(input, info);
			print_error("Exec error\n", 0); // TODO: get correct error
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < n_process)
		waitpid(pid[i++], NULL, 0);
	return (0);
}
