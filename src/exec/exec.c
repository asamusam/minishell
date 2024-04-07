/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/07 13:51:59 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	handle_last_redirection(t_pipe *pipet, t_command *command)
{
	if (command->file_in >= 0)
	{
		dup2(command->file_in, STDIN_FILENO);
		close(command->file_in);
	}
	else
		dup2(pipet->prev_pipe, STDIN_FILENO);
	close(pipet->prev_pipe);
	if (command->file_out >= 0)
	{
		dup2(command->file_out, STDOUT_FILENO);
		close(command->file_out);
	}
	return (0);
}

int	handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int		status;

	if (!command)
		return (1);
	status = 0;
	if ((pipet->pid = fork()) == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		// dup2(pipet->prev_pipe, STDIN_FILENO);
		// close(pipet->prev_pipe);
		handle_last_redirection(pipet, command);
		status = handle_input(command, info);
	}
	else
	{
		close(pipet->prev_pipe);
		waitpid(pipet->pid, NULL, 0);
	}
	return (status);
}

/*
 * Create last process, close remaining pipes, executes cmd
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */
int	last_process(t_command *command, t_info *info, t_pipe *pipet)
{
	int		status;

	if (!command)
		return (1);
	status = 0;
	if (!is_buitin(command->args->content))
		handle_lst_cmd_process(pipet, command, info);
	else
	{
		//dup2(pipet->prev_pipe, STDIN_FILENO);
		//close(pipet->prev_pipe);
		//TODO: double check if redirection in with builtin. f.i. echo < /home/mmughedd/minishell/test1.txt
		handle_last_redirection(pipet, command);
		status = handle_builtin(command, info);
	}
	close(command->file_out);
	return (status);
}


int	handle_redirections(t_pipe *pipet, t_command *command)
{
	close (pipet->pipefd[0]);
	if (command->file_in)
	{
		dup2 (command->file_in, STDIN_FILENO);
		close(command->file_in);
	}
	close(pipet->prev_pipe);
	if (command->file_out)
	{
		dup2 (command->file_out, STDOUT_FILENO);
		close(command->file_out);
	}
	close (pipet->pipefd[1]);
	// dup2 (pipet->pipefd[1], STDOUT_FILENO);
	// close (pipet->pipefd[1]);
	// dup2 (pipet->prev_pipe, STDIN_FILENO);
	// close (pipet->prev_pipe);
	return (0);
}

int	handle_cmd_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int	status;

	if (!command)
		return (1);
	status = 0;
	if ((pipet->pid = fork()) == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		// close (pipet->pipefd[0]);
		// dup2 (pipet->pipefd[1], STDOUT_FILENO);
		// close (pipet->pipefd[1]);
		// dup2 (pipet->prev_pipe, STDIN_FILENO);
		// close (pipet->prev_pipe);
		handle_redirections(pipet, command);
		status = handle_input(command, info);
	}
	else
	{
		close(pipet->pipefd[1]);
		close(pipet->prev_pipe);
		pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, NULL, 0);
	}
	return (status);
}

int	handle_bltn_process(t_pipe *pipet, t_command *command, t_info *info)
{
	int	status;

	if (!command)
		return (1);
	status = 0;
	if ((pipet->pid = fork()) == -1)
		return (print_error("Fork error\n", 0));
	if (pipet->pid == 0)
	{
		close (pipet->pipefd[0]);
		close (pipet->pipefd[1]);
		close (pipet->prev_pipe);
	}
	else
	{
		close(pipet->pipefd[0]); //
		dup2 (pipet->pipefd[1], STDOUT_FILENO);
		close(pipet->pipefd[1]);
		dup2 (pipet->prev_pipe, STDIN_FILENO);
		close(pipet->prev_pipe);
		//pipet->prev_pipe = pipet->pipefd[0];
		waitpid(pipet->pid, NULL, 0);
		status = handle_builtin(command, info);
	}
	return (status);
}

/*
 * Create processes, pipes and executes commands
 *
 * Arguments:
 * - command - command structure
 * - info - info structure
 * - prev_pipe - previous pipe,
 * 
 * Returns:
 * Status
 */
int create_process(t_command *command, t_info *info, t_pipe *pipet)
{
	int		status;

	if (!command)
		return (1);
	status = 0;
	if (pipe(pipet->pipefd) == -1)
		return (print_error("Pipe error\n", 0));
	if (!is_buitin(command->args->content))
		handle_cmd_process(pipet, command, info);
	else
		handle_bltn_process(pipet, command, info);
	return (status);
}

/*
 * Loops through commands list and creates processes accordingly
 *
 * Arguments:
 * - commands - list of commands/processes
 * - info - info structure
 * 
 * Returns:
 * Status
 */
int	exec(t_list *commands, t_info *info)
{
	int		prev_pipe;
	int		status;
	t_list	*current;
	t_pipe	*pipet;

	pipet = malloc(sizeof(t_pipe));
	if (!pipet)
		return (print_error("malloc error\n", 0));
	pipet->prev_pipe = dup(0);// initialized with any valid fd to avoid error, since first child doesn't need prev_pipe
	current = commands;
	status = 0;
	if (current && current->next)
			info->is_multiple_proc = 1;
	while (current && current->next && !info->exit_flag)
	{
		status = create_process((t_command *)(current->content), info, pipet);
		current = current->next;
	}
	if (!info->exit_flag)
		status = last_process((t_command *)(current->content), info, pipet);
	free(pipet);
	return (status);
}

int	update_envstr(t_info *info)
{
	int		i;
	int		len;
	char	*tmp;
	t_list	*current;

	i = 0;
	current = info->envp_list;
	while (info->envp[i])
		free(info->envp[i++]);
	free(info->envp);
	len = ft_lstsize(info->envp_list);
	info->envp = malloc(sizeof(char *) * (len + 1));
	if (!info->envp)
		return(print_error("malloc error\n", 0));
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(((t_envp *)current->content)->key, "=");
		if (((t_envp *)current->content)->value)	
			info->envp[i] = ft_strjoin(tmp, ((t_envp *)current->content)->value);
		else
			info->envp[i] = ft_strdup(tmp);
		free(tmp);
		i++;
		current = current->next;
	}
	return (0);
}

char	**copy_envp(char **envp)
{
	int	i;
	char	**envp_copy;

	if (!*envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	envp_copy = malloc(sizeof(char *) * (i + 1));
	if (!envp_copy)
	{
		print_error("malloc error\n", 0);
		return (NULL);
	}
	envp_copy[i] = NULL;
	i = 0;
	while (envp[i])
	{
		envp_copy[i] = ft_strdup(envp[i]);
		if (!envp_copy[i++])
		{
			free(envp_copy);
			return (envp_copy);
		}
	}
	return (envp_copy);
}

void	set_envp(t_info *info, char **envp)
{
	char	**keyval;
	char	*key;
	char	*value;
	char	**current;
	
	current = envp;
	while (*current)
	{
		keyval = ft_split(*current, '='); // TODO: handle split if not =
		key = ft_strdup(keyval[0]);
		if (keyval[1])
			value = ft_strdup(keyval[1]);
		else
			value = NULL;
		if (!info->envp_list) // if no env set
			info->envp_list = create_envp_node(key, value);
		else
			check_envs(info, key, value);
		free_split(keyval);
		free(key);
		free(value);
		current++;
	}
}

///////////////////////////////////////////////////////////////////////
// For testing/////////////////////
///////////////////////////////////////////////////////////////////////

void printenvv(t_list *lst)
{
	t_list *list;
	list = lst;
	while (list)
	{
		printf("%s=%s\n", ((t_envp *)list->content)->key, ((t_envp *)list->content)->value);
		list = list->next;
	}
}

void printenvst(char **envp)
{
	int i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
}

t_command *create_command(t_list *args, int in, int out)
{
	t_command *command;

	command = malloc(sizeof(t_command));
	command->args = args;
	command->file_in = in;
	command->file_out = out;
	return (command);
}

t_info	*create_info(char **envp)
{
	t_info	*info;
	char	*paths;

	info = malloc(sizeof(t_info));
	if (!info)
	{
		print_error("Malloc error\n", 0);
		return (NULL);
	}
	info->envp = copy_envp(envp);
	info->pwd = NULL;
	info->oldpwd = NULL;
	info->home = NULL;
	paths = ft_strdup(getenv("PATH"));
	info->path = ft_split(paths, ':');
	free(paths);
	info->envp_list = NULL;
	info->exit_code = 0;
	info->is_multiple_proc = 0;
	return (info);
}

void	set_pwds(t_info *info)
{
	t_list	*enpv_list;
	char	*key;

	enpv_list = info->envp_list;
	while (enpv_list)
	{
		key = ((t_envp *)enpv_list->content)->key;
		if (key && !ft_strcmp(key, "PWD"))
			info->pwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "OLDPWD"))
			info->oldpwd = ((t_envp *)enpv_list->content)->value;
		else if (key && !ft_strcmp(key, "HOME"))
			info->home = ((t_envp *)enpv_list->content)->value;
		enpv_list = enpv_list->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_info *info;

	info = create_info(envp);
	set_envp(info, envp);
	set_pwds(info);
	char *cmd1[] = {"ls", "-l", "test.txt"};//{"export", "aaas=eewrfweno"};//
	char *cmd2[] = {"cat", "testttttt.txt"};
	//char *cmd3[] = {"export", "3=33"};
	t_list *args1, *args2, *args3, *main_list, *mainlist2;
	t_command *comm1, *comm2, *comm3;
	//info->envp_list = create_envp_node("aaas", "00");
	args1 = ft_lstnew((void *)cmd1[0]);
	args1->next = ft_lstnew((void *)cmd1[1]);
	//args1->next->next = ft_lstnew((void *)cmd1[2]);
	args2 = ft_lstnew((void *)cmd2[0]);
	//args2->next = ft_lstnew((void *)cmd2[1]);
	// args2->next->next = ft_lstnew((void *)cmd2[2]);
	//args3 = ft_lstnew((void *)cmd3[0]);
	//args3->next = ft_lstnew((void *)cmd3[1]);
	int fd1, fd2, fd3;
	fd1 = open("/home/mmughedd/minishell/test1.txt", O_RDONLY);
	fd2 = open("/home/mmughedd/minishell/test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	fd3 = open("/home/mmughedd/minishell/test3.txt", O_WRONLY | O_CREAT | O_APPEND, 0664);
	comm1 = create_command(args1, -1, fd3);
	comm2 = create_command(args2, fd3, -1);
	//comm3 = create_command(args3, NULL, NULL, 0);
	main_list = ft_lstnew((void *)comm1);
	mainlist2 = ft_lstnew((void *)comm2);
	//main_list->next = ft_lstnew((void *)comm2);
	//main_list->next->next = ft_lstnew((void *)comm3);
	exec(main_list, info);
	//exec(mainlist2, info);
	//printenvv(info->envp_list);
	//printenvst(info->envp);
	free_info(info);
	ft_lstclear(&(main_list), &del_lst_content);
}
/*
// gdb
// set follow-fork-mode child: gdb will automatically switch to debugging the child process when a fork() call is encountered.
// set follow-fork-mode parent: gdb will stay with the parent process and not switch to the child process.

// Valgrind with pipes and forks

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./a.out

// compiling

cc -g utils.c builtins/builtins_utils.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/export_utils.c builtins/pwd.c builtins/unset.c exec/exec.c exec/exec_utils.c free/free.c ../libft/libft.a -I ../include/

*/