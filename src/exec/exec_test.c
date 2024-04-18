/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/18 13:37:37 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	redir_stdin(int	*original_stdin, int fd)
{
	if (fd == -1)
		return (SUCCESS);
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin == -1)
		return (print_error(DUP_ERROR, PERROR));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(fd) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	redir_stdout(int *original_stdout, int fd)
{
	if (fd == -1)
		return (SUCCESS);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdout == -1)
		return (print_error(DUP_ERROR, PERROR));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(fd) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	restore_stdin(int original_stdin, int fd)
{
	if (fd == -1)
		return (SUCCESS);
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdin) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

int	restore_stdout(int original_stdout, int fd)
{
	if (fd == -1)
		return (SUCCESS);
	if (dup2(original_stdout, STDOUT_FILENO) == -1)
		return (print_error(DUP2_ERROR, PERROR));
	if (close(original_stdout) == -1)
		return (print_error(CLOSE_ERROR, PERROR));
	return (SUCCESS);
}

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
	if (command->args)
		status = handle_input(command, minishell);
	else
		status = SUCCESS;
	if (restore_stdin(original_stdin, command->file_in) == FAIL)
		exit(FAIL);
	if (restore_stdout(original_stdout, command->file_out) == FAIL)
		exit(FAIL);
	exit(status);
}

int	parent(pid_t pid)
{
	int		child_status;
	void	(*parent_handler)(int);

	parent_handler = signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &child_status, 0) == -1)
		return (print_error(WAIT_ERROR, PERROR));
	signal(SIGINT, parent_handler);
	if (WIFEXITED(child_status))
		return (WEXITSTATUS(child_status));
	else
	{
		if (WTERMSIG(child_status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(child_status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
		return (128 + WTERMSIG(child_status));
	}
}

int	run_builtin(t_command *command, t_info *minishell)
{
	int		original_stdin;
	int		original_stdout;
	int		status;

	if (redir_stdin(&original_stdin, command->file_in) == FAIL)
		return (FAIL);
	if (redir_stdout(&original_stdout, command->file_out) == FAIL)
		return (FAIL);
	status = handle_builtin(command, minishell);
	if (restore_stdin(original_stdin, command->file_in) == FAIL)
		return (FAIL);
	if (restore_stdout(original_stdout, command->file_out) == FAIL)
		return (FAIL);
	return (status);
}

int	run_command(t_command *command, t_info *minishell, int in, int out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (print_error(FORK_ERROR, PERROR));
	if (pid == 0)
		child(command, minishell, in, out);
	return (parent(pid));
}

void	free_pipes(int **pipes, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(pipes[i++]);
	free(pipes);
}

int	close_pipes(int **pipes, int i, int i_max)
{
	if (i > 0)
	{
		if (close(pipes[i - 1][0]) == -1)
			return (FAIL);
	}
	if (i < i_max)
	{
		if (close(pipes[i][1]) == -1)
			return (FAIL);
	}
	if (i == i_max)
	{
		if (close(pipes[i][0]) == -1)
			return (FAIL);
		if (close(pipes[i][1]) == -1)
			return (FAIL);
	}
	return (SUCCESS);
}

int	free_pipes_return_fail(int **pipes, int size, char *error_type)
{
	if (pipes)
		free_pipes(pipes, size);
	return (print_error(error_type, PERROR));
}

int	run_single_command(t_list *commands, t_info *minishell)
{
	int			status;
	t_command	*command;

	command = (t_command *)commands->content;
	if (is_builtin(command->args->content))
		status = run_builtin(command, minishell);
	else
		status = run_command(command, minishell, -1, -1);
	return (status);
}

int	run_pipe_segment(int i, int **pipes, t_list *commands, t_info *minishell)
{
	t_command	*command;
	int			status;

	command = (t_command *)commands->content;
	if (i > 0 && commands->next)
		status = run_command(command, minishell, pipes[i - 1][0], pipes[i][1]);
	else if (i == 0)
		status = run_command(command, minishell, -1, pipes[i][1]);
	else
		status = run_command(command, minishell, pipes[i - 1][0], -1);
	return (status);
}

int	run_pipeline(t_list *commands, t_info *minishell)
{
	int	**pipes;
	int	i;
	int	status;
	int	size;

	i = 0;
	status = SUCCESS;
	size = ft_lstsize(commands);
	pipes = malloc(sizeof(int *) * size);
	while (commands && !status)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes || !pipes[i])
			return (free_pipes_return_fail(pipes, i, MALLOC_ERROR));
		if (pipe(pipes[i]) == -1)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		status = run_pipe_segment(i, pipes, commands, minishell);
		if (close_pipes(pipes, i, size - 1) == FAIL)
			return (free_pipes_return_fail(pipes, i, PIPE_ERROR));
		commands = commands->next;
		i ++;
	}
	free_pipes(pipes, i);
	return (status);
}

int	exec_test(t_list *commands, t_info *minishell)
{
	int	status;

	if (!((t_command *)commands->content)->args)
		return (SUCCESS);
	if (!commands->next)
		status = run_single_command(commands, minishell);
	else
		status = run_pipeline(commands, minishell);
	return (status);
}
