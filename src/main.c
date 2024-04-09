/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/09 15:12:25 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(t_info *minishell)
{
	char	*line;
	t_list	*tokens;
	t_list	*commands;

	line = readline("-->");
	while (line)
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
					minishell->exit_code = exec(commands, minishell);
				// executor goes here (if commands)
				//ft_lstiter(commands, print_command); // temporary
				//ft_lstclear(&commands, free_command); // temporary
			}

				
		}
		if (g_signal == SIGINT)
			minishell->exit_code = g_signal + 128;
		free(line);
		line = readline("-->");
	}
	printf("exit\n");
	ft_lstclear(&minishell->envp_list, free_envvar);
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
