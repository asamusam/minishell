/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/29 18:54:49 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(t_info *minishell)
{
	char	*line;
	t_list	*tokens;

	set_signal_handler();
	line = readline("-->");
	while (line)
	{
		if (*line)
		{
			add_history(rl_line_buffer);
			tokens = lexer(line);
			if (tokens)
				parser(tokens, minishell);
		}
		free(line);
		line = readline("-->");
	}
	printf("exit\n");
	free_split(minishell->envp);
	ft_lstclear(&minishell->envp_list, free_envvar);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_info	minishell;

	(void)av;
	if (ac == 1)
	{
		init_envp(envp, &minishell);
		minishell.return_code = 0;
		shell_loop(&minishell);
	}
	else
	{
		ft_putstr_fd("We don't accept arguments for now.", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
