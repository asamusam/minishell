/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/09 08:55:09 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	shell_loop(char **envp)
{
	char	**args;
	char	*line;

	(void)envp;
	set_signal_handler();
	line = readline("-->");
	while (line)
	{
		if (*line)
		{
			add_history(rl_line_buffer);
			printf("%s\n", rl_line_buffer);
			args = ft_split(rl_line_buffer, ' ');
			free_split(args);
		}
		free(line);
		line = readline("-->");
	}
	printf("exit\n");
	rl_clear_history();
	exit(EXIT_SUCCESS);
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
