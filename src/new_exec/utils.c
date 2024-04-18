/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:33:16 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/18 14:37:17 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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