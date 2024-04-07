/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:32:40 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/04 17:07:29 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

volatile sig_atomic_t	g_signal;

void	signal_handler(int signal)
{
	g_signal = signal;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	(void)signal;
}

void	set_signal_handler(void)
{
	struct sigaction	sa;
	struct sigaction	ignore;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &ignore, NULL);
}
