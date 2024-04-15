/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:55:35 by asamuilk          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/15 10:43:41 by mmughedd         ###   ########.fr       */
=======
/*   Updated: 2024/04/10 15:41:45 by asamuilk         ###   ########.fr       */
>>>>>>> free
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(t_info *minishell)
{
	char	*line;
	t_list	*tokens;
	t_list	*commands;
	char *prompt; //temp

<<<<<<< HEAD
	if (minishell->pwd)//temp
		prompt = ft_strjoin(minishell->pwd, ">"); //temp
	else//temp
		prompt = "-->";//temp

	line = readline(prompt); //temp 
	//line = readline("-->");
=======
	line = readline("-->");
>>>>>>> free
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
		printf("exit code: %d\n", minishell->exit_code);//tmp
		if (g_signal == SIGINT)
			minishell->exit_code = g_signal + 128;
		free(line);
<<<<<<< HEAD
		if (minishell->pwd && !minishell->exit_flag)//temp
		{											//temp
			free(prompt);							//temp
			prompt = ft_strjoin(minishell->pwd, ">"); //temp
		}//temp
		else if (!minishell->exit_flag)//temp
			prompt = "-->";//temp
		if (!minishell->exit_flag) // temp
			line = readline(prompt);// temp
		//if (!minishell->exit_flag)
		//	line = readline("-->");
=======
		if (!minishell->exit_flag)
			line = readline("-->");
>>>>>>> free
	}
	free(prompt); //temp
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
