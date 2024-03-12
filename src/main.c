/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/12 19:21:45 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(char **envp)
{
	char	*line;
	t_list	*tokens;

	(void)envp;
	set_signal_handler();
	line = readline("-->");
	while (line)
	{
		if (*line)
		{
			add_history(rl_line_buffer);
			tokens = lexer(line);
			if (tokens)
				ft_lstiter(tokens, print_token);
		}
		ft_lstclear(&tokens, free);
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
