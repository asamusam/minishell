/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/04 19:08:30 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(char **envp)
{
	char	*line;
	char	**args;

	(void)envp;
	while (1)
	{
		line = readline("-->");
		if (line)
		{
			add_history(line);
			args = ft_split(line, ' ');
			free(line);
			free_split(args);
		}
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{

	(void)av;
	if (ac == 1)
		shell_loop(envp);
	else
	{
		ft_putstr_fd("We don't accept arguments for now.", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
