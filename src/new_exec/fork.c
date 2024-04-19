/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:31:52 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/19 19:09:03 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// void	child_signal_handler(int signal)
// {
// 	if (signal == SIGINT)
// 		printf("\n");
// 	else if (signal == SIGQUIT)
// 		ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
// }

void	child(t_command *command, t_info *minishell, int in, int out)
{
	int		original_stdin;
	int		original_stdout;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (redir_stdin(&original_stdin, command->file_in) == FAIL)
		exit(FAIL);
	else if (in != -1 && dup2(in, STDIN_FILENO) == -1)
		exit(FAIL);
	if (redir_stdout(&original_stdout, command->file_out) == FAIL)
		exit(FAIL);
	else if (out != -1 && dup2(out, STDOUT_FILENO) == -1)
		exit(FAIL);
	if (command->args && !is_builtin(command->args->content))
		status = handle_input(command, minishell);
	else if (command->args)
		status = handle_builtin(command, minishell);
	else
		status = SUCCESS;
	if (restore_stdin(original_stdin, command->file_in) == FAIL)
		exit(FAIL);
	if (restore_stdout(original_stdout, command->file_out) == FAIL)
		exit(FAIL);
	exit(status);
}

int	wait_last(t_info *minishell, int last)
{
	int		child_status;
	//void	(*parent_handler)(int);

	if (waitpid(minishell->processes[last], &child_status, 0) == -1)
		return (print_error(WAIT_ERROR, PERROR));
	if (WIFEXITED(child_status))
		return (WEXITSTATUS(child_status));
	else
		return (128 + WTERMSIG(child_status));
}

int	run_command(t_command *command, t_info *minishell, int in, int out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (print_error(FORK_ERROR, PERROR));
	if (pid == 0)
		child(command, minishell, in, out);
	return (pid);
}