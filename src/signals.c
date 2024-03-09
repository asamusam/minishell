/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:32:40 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/09 06:51:15 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

// commmented out because it throws an error

// void	set_signal_handler(void)
// {
// 	struct sigaction	sa;
// 	struct sigaction	ignore;

// 	sa.sa_handler = signal_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;

// 	ignore.sa_handler = SIG_IGN;
// 	sigemptyset(&ignore.sa_mask);
// 	ignore.sa_flags = SA_RESTART;

// 	sigaction(SIGINT, &sa, NULL);
// 	sigaction(SIGQUIT, &ignore, NULL);
// }
