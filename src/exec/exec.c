/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/02 13:09:38 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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
int	last_process(t_command *command, t_info *info, int prev_pipe)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!is_buitin(command->args->content))
	{
		if ((pid = fork()) == -1)
			return (print_error("Fork error\n", 0));
		if (pid == 0)
		{
			dup2(prev_pipe, STDIN_FILENO); // TODO: check for redirection
			close(prev_pipe);
			status = handle_input(command, info);
		}
		else
		{
			close(prev_pipe);
			waitpid(pid, NULL, 0);
		}
	}
	else
	{
		close(prev_pipe);
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
int create_process(t_command *command, t_info *info, int *prev_pipe)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	status = 0;
	if (pipe(pipefd) == -1)
		return (print_error("Pipe error\n", 0));
	if ((pid = fork()) == -1)
		return (print_error("Fork error\n", 0));
	if (!is_buitin(command->args->content))
	{
		if (pid == 0)
		{
			close (pipefd[0]);
			dup2 (pipefd[1], STDOUT_FILENO);
			close (pipefd[1]);
			dup2 (*prev_pipe, STDIN_FILENO);
			close (*prev_pipe);
			status = handle_input(command, info);
		}
		else
		{
			close(pipefd[1]);
			close(*prev_pipe);
			*prev_pipe = pipefd[0];
			waitpid(pid, NULL, 0);
		}
	}
	else
	{
		if (pid == 0)
		{
			close (pipefd[0]);
			close (pipefd[1]);
			close (*prev_pipe);
		}
		else
		{
			dup2 (pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			close(*prev_pipe);
			*prev_pipe = pipefd[0];
			waitpid(pid, NULL, 0);
			status = handle_builtin(command, info);
		}
	}
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
	
	prev_pipe = dup(0);// initialized with any valid fd to avoid error, since first child doesn't need prev_pipe
	current = commands;
	status = 0;
	if (current && current->next)
			info->is_multiple_proc = 1;
	while (current && current->next && !info->exit_flag)
	{
		status = create_process((t_command *)(current->content), info, &prev_pipe);
		current = current->next;
	}
	if (!info->exit_flag)
		status = last_process((t_command *)(current->content), info, prev_pipe);
	return (status);
}

char	**copy_envp(char**envp)
{
	int	i;
	char	**envp_copy;

	if (!*envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	envp_copy = malloc(sizeof(char *) * (i + 1));
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

t_command *create_command(t_list *args, char *in, char *out, int flag)
{
	t_command *command;

	command = malloc(sizeof(t_command));
	command->args = args;
	command->file_in = in;
	command->file_out = out;
	command->file_out_flag = flag;
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
	info->exit_flag = 0;
	info->is_multiple_proc = 0;
	return (info);
}

void	del_cmd_content(void *content)
{
	//free(content); TODO: check if allocated
	content = NULL;
}


void	del_lst_content(void *content)
{
	t_command	*cmd;

	cmd = (t_command *)content;
	free(cmd->file_in);
	free(cmd->file_out);
	ft_lstclear(&(cmd->args), del_cmd_content);
	free(cmd);
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
	char *cmd1[] = {"wc", "-c", "test.txt"};//{"export", "aaas=eewrfweno"};//
	//char *cmd1[] = {"wc", "-c", "test.txt"};
	char *cmd2[] = {"exit", "1"};
	// char *cmd3[] = {"export", "3=33"};
	t_list *args1, *args2, *args3, *main_list;
	t_command *comm1, *comm2, *comm3;
	//info->envp_list = create_envp_node("aaas", "00");
	args1 = ft_lstnew((void *)cmd1[0]);
	args1->next = ft_lstnew((void *)cmd1[1]);
	args1->next->next = ft_lstnew((void *)cmd1[2]);
	args2 = ft_lstnew((void *)cmd2[0]);
	args2->next = ft_lstnew((void *)cmd2[1]);
	// args2->next->next = ft_lstnew((void *)cmd2[2]);
	// args3 = ft_lstnew((void *)cmd3[0]);
	// args3->next = ft_lstnew((void *)cmd3[1]);
	comm1 = create_command(args1, NULL, NULL, 0);
	comm2 = create_command(args2, NULL, NULL, 0);
	// comm3 = create_command(args3, NULL, NULL, 0);
	main_list = ft_lstnew((void *)comm1);
	main_list->next = ft_lstnew((void *)comm2);
	//main_list->next->next = ft_lstnew((void *)comm3);
	exec(main_list, info);
	//printenvv(info->envp_list);
	free_info(info);
	ft_lstclear(&(main_list), &del_lst_content);
}
/*
// gdb
// set follow-fork-mode child: gdb will automatically switch to debugging the child process when a fork() call is encountered.
// set follow-fork-mode parent: gdb will stay with the parent process and not switch to the child process.

// Valgrind with pipes and forks
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./a.out

// compiling
// cc -g utils.c builtins/builtins_utils.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/pwd.c builtins/unset.c exec/exec.c exec/exec_utils.c ../include/exec.h ../libft/libft.a
cc -g utils.c builtins/builtins_utils.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c exec/exec.c exec/exec_utils.c  ../libft/libft.a -I ../include/
//TODO: signals during process?
*/