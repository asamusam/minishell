/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:33:16 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/23 17:44:25 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	waiting_signal_handler(int signal)
{
	if (signal == SIGINT)
		printf("\n");
	else if (signal == SIGQUIT)
		printf("Quit (core dumped)\n");
}

int	wait_for_children(t_info *minishell)
{
	int		status;
	void	(*old_handler)(int);

	old_handler = signal(SIGINT, waiting_signal_handler);
	signal(SIGQUIT, waiting_signal_handler);
	if (waitpid(minishell->last_prc, &status, 0) == -1)
		return (print_error(WAIT_ERROR, PERROR));
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
		status = 128 + WTERMSIG(status);
	while (wait(NULL) != -1)
		continue ;
	signal(SIGINT, old_handler);
	signal(SIGQUIT, SIG_IGN);
	return (status);
}

void	close_pipes(t_info *msh, int i)
{
	if (i == 0)
		close(msh->pipes[i][1]);
	else if (i > 0 && i < msh->psize)
	{
		close(msh->pipes[i - 1][0]);
		close(msh->pipes[i][1]);
	}
	else
		close(msh->pipes[i - 1][0]);
}

void	free_pipes(int **pipes, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(pipes[i++]);
	free(pipes);
}

int	free_pipes_fail(int **pipes, int size, char *error_type)
{
	if (pipes)
		free_pipes(pipes, size);
	return (print_error(error_type, PERROR));
}
