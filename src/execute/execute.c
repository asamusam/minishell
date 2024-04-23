/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 21:39:09 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/23 14:08:46 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	run_builtin(t_command *command, t_info *minishell)
{
	int		in;
	int		out;
	int		status;

	if (backup_stdin_stdout(&in, &out, command) == FAIL)
		return (FAIL);
	if (redir_stdin(command->file_in, -1) == FAIL)
		return (FAIL);
	if (redir_stdout(command->file_out, -1) == FAIL)
		return (FAIL);
	status = handle_builtin(command, minishell);
	if (command->file_in && restore_stdin(in) == FAIL)
		return (FAIL);
	if (command->file_out && restore_stdout(out) == FAIL)
		return (FAIL);
	return (status);
}

static int	run_single_command(t_list *commands, t_info *minishell)
{
	t_command	*command;

	command = (t_command *)commands->content;
	minishell->psize = 0;
	if (is_builtin(command->args->content))
		return (run_builtin(command, minishell));
	else
		minishell->last_prc = run_command(command, minishell, 0);
	return (wait_for_children(minishell));
}

static int	create_pipes(t_info *minishell)
{
	int	i;

	i = 0;
	minishell->pipes = malloc(sizeof(int *) * minishell->psize);
	if (!minishell->pipes)
		return (print_error(MALLOC_ERROR, PERROR));
	while (i < minishell->psize)
	{
		minishell->pipes[i] = malloc(2 * sizeof(int));
		if (!minishell->pipes[i])
			return (free_pipes_fail(minishell->pipes, i, MALLOC_ERROR));
		i ++;
	}
	return (SUCCESS);
}

static int	run_pipeline(t_list *commands, t_info *msh)
{
	int			i;
	t_command	*command;

	i = 0;
	msh->psize = ft_lstsize(commands) - 1;
	if (create_pipes(msh) == FAIL)
		return (FAIL);
	while (commands)
	{
		command = (t_command *)commands->content;
		if (i < msh->psize && pipe(msh->pipes[i]) == -1)
			return (free_pipes_fail(msh->pipes, msh->psize, PIPE_ERROR));
		msh->last_prc = run_command(command, msh, i);
		if (msh->last_prc == -1)
			return (free_pipes_fail(msh->pipes, msh->psize, FORK_ERROR));
		if (close_pipes(msh, i) == FAIL)
			return (free_pipes_fail(msh->pipes, msh->psize, CLOSE_ERROR));
		commands = commands->next;
		i ++;
	}
	free_pipes(msh->pipes, msh->psize);
	return (SUCCESS);
}

int	execute(t_list *commands, t_info *minishell)
{
	if (!((t_command *)commands->content)->args && !commands->next)
		return (SUCCESS);
	if (!commands->next)
		return (run_single_command(commands, minishell));
	else if (run_pipeline(commands, minishell) == FAIL)
		return (FAIL);
	return (wait_for_children(minishell));
}
