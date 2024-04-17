/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:00:06 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/17 14:33:48 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_parent(t_pipe *pipet)
{
	int		child_status;
	void	(*parent_handler)(int);

	parent_handler = signal(SIGINT, SIG_IGN);
	if (close (pipet->pipefd[1]) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	if (close (pipet->prev_pipe) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	pipet->prev_pipe = pipet->pipefd[0];
	if (waitpid(pipet->pid, &child_status, 0) == -1)
		return (print_error(WAIT_ERROR, PERROR));
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

int	handle_last_parent(t_pipe *pipet)
{
	int		child_status;
	void	(*parent_handler)(int);

	parent_handler = signal(SIGINT, SIG_IGN);
	if (close (pipet->prev_pipe) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	if (waitpid(pipet->pid, &child_status, 0) == -1)
		return (print_error(WAIT_ERROR, PERROR));
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
