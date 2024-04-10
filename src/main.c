/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/10 15:41:45 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(t_info *minishell)
{
	char	*line;
	t_list	*tokens;
	t_list	*commands;

	line = readline("-->");
	while (!minishell->exit_flag && line)
	{
		if (*line)
		{
			g_signal = 0;
			add_history(rl_line_buffer);
			tokens = lexer(line);
			if (tokens)
			{
				commands = parser(tokens, minishell);
				if (commands)
				{
					minishell->exit_code = exec(commands, minishell);
					ft_lstclear(&commands, free_command);
				}
			}
		}
		if (g_signal == SIGINT)
			minishell->exit_code = g_signal + 128;
		free(line);
		if (!minishell->exit_flag)
			line = readline("-->");
	}
	printf("exit\n");
	free_minishell_info(minishell);
	rl_clear_history();
	exit(minishell->exit_code);
}

int	main(int ac, char **av, char **envp)
{
	t_info	minishell;

	(void)av;
	if (ac == 1)
	{
		set_signal_handler();
		init(envp, &minishell);
		shell_loop(&minishell);
	}
	else
	{
		ft_putstr_fd("We don't accept arguments for now.", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
