/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/17 12:43:05 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Create last process, close remaining pipes, executes cmd
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */

int	ft_last_pipe(t_command *command, t_info *info, int *prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if ((pid = fork()) == -1)
	{
		print_error("Fork error\n", 0); // TODO: check error
		return (1);
	}
	if (pid == 0)
	{
		dup2(prev_pipe, STDIN_FILENO); // TODO: check for redirection
		close(prev_pipe);
		if (!is_buitin(command->args->content))
			handle_builtin(command, info);
		else
			handle_input(command, info);
	}
	else
	{
		close(prev_pipe);
		waitpid(pid, NULL, 0);
	}
	return (0);
}

/*
 * Create processes, pipes and executes commands
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */

int ft_pipe(t_command *command, t_info *info, int *prev_pipe)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		print_error("Pipe error\n", 0); // TODO: check error, make sure to close previous pipes
		return (1);
	}
	if ((pid = fork()) == -1)
	{
		print_error("Fork error\n", 0); // TODO: check error
		return (1);
	}
	if (pid == 0) // child process
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO); // write TODO: check for redirection
		close (pipefd[1]);
		dup2(*prev_pipe, STDIN_FILENO); // read TODO: check for redirection
		close(*prev_pipe);
		if (!is_buitin(command->args->content))
			handle_builtin(command, info);
		else
			handle_input(command, info);
	}
	else
	{
		close(pipefd[1]);
		close(*prev_pipe);
		*prev_pipe = pipefd[0];
		waitpid(pid, NULL, 0);
	}
	return (0);
}

/*
 * Loops through commands list and creates processes accordingly
 *
 * Arguments:
 * - commands - list of commands/processes
 * - info - info structure
 * 
 * Returns:
 * TBD
 */

void	exec(t_list *commands, t_info *info)
{
	int		prev_pipe;
	t_list	*current;

	prev_pipe = dup(0); // initialized with any valid fd to avoid error, since first child doesn't need prev_pipe
	current = commands;
	while (current && current->next)
	{
		ft_pipe(current->content, info, &prev_pipe);
		current = current->next;
	}
	ft_last(current->content, info, prev_pipe);
	// TODO: free(commands) 
}

// Valgrind with pipes and forks
//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./a.out