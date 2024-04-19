/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:33:16 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/20 00:07:16 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	waiting_signal_handler(int signal)
{
	if (signal == SIGINT)
		printf("\n");
	else if (signal == SIGQUIT)
		printf("Quit (core dumped)\n");
}

void	wait_for_children(t_info *minishell)
{
	int	status;

	signal(SIGINT, waiting_signal_handler);
	signal(SIGQUIT, waiting_signal_handler);
	if (waitpid(minishell->last_prc, &status, 0) == -1)
		minishell->exit_code = print_error(WAIT_ERROR, PERROR);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
		status = 128 + WTERMSIG(status);
	while (wait(NULL) != -1)
		continue ;
	minishell->exit_code = status;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	close_pipes(int **pipes, int i, int i_max)
{
	if (i > 0)
	{
		if (close(pipes[i - 1][0]) == -1)
			return (FAIL);
	}
	if (i < i_max)
	{
		if (close(pipes[i][1]) == -1)
			return (FAIL);
	}
	if (i == i_max)
	{
		if (close(pipes[i][0]) == -1)
			return (FAIL);
		if (close(pipes[i][1]) == -1)
			return (FAIL);
	}
	return (SUCCESS);
}

void	free_pipes(int **pipes, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(pipes[i++]);
	free(pipes);
}

int	free_pipes_return_fail(int **pipes, int size, char *error_type)
{
	if (pipes)
		free_pipes(pipes, size);
	return (print_error(error_type, PERROR));
}
