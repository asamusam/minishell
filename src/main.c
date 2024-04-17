/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/17 18:38:40 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_clean(t_info *minishell)
{
	printf("exit\n");
	free_minishell_info(minishell);
	rl_clear_history();
	exit(minishell->exit_code);
}

void	run_line(char *line, t_info *minishell)
{
	t_list	*tokens;
	t_list	*commands;

	g_signal = 0;
	commands = NULL;
	add_history(line);
	tokens = lexer(line);
	if (tokens)
		commands = parser(tokens, minishell);
	if (commands)
		minishell->exit_code = exec(commands, minishell);
	else
		minishell->exit_code = FAIL;
}

void	shell_loop(t_info *minishell)
{
	char	*line;

	while (!minishell->exit_flag)
	{
		line = readline("-->");
		if (!line)
			exit_clean(minishell);
		else if (!*line && g_signal)
			minishell->exit_code = g_signal + 128;
		else
			run_line(line, minishell);
		free(line);
	}
	exit_clean(minishell);
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
