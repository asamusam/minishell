/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/31 15:27:18 by mmughedd         ###   ########.fr       */
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

	if (!is_buitin(command->args->content))
	{
		if ((pid = fork()) == -1)
		{
			print_error("Fork error\n", 0); // TODO: check error
			return (1);
		}
		if (pid == 0)
		{
			dup2(prev_pipe, STDIN_FILENO); // TODO: check for redirection
			close(prev_pipe);
			handle_input(command, info);
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
		handle_builtin(command, info);
	}
	return (0);
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

		info->is_multiple_proc = 1;
		if (pipe(pipefd) == -1)
		{
			print_error("Pipe error\n", 0); // TODO: check error, make sure to close previous pipes
			return (1);
		}
		if ((pid = fork()) == -1)
		{
			print_error("Fork error\n", 0); // TODO: check error
			return (1);
		}
		if (!is_buitin(command->args->content))
		{
			if (pid == 0)
			{
				close (pipefd[0]);
				dup2 (pipefd[1], STDOUT_FILENO);
				close (pipefd[1]);
				dup2 (*prev_pipe, STDIN_FILENO);
				close (*prev_pipe);
				handle_input(command, info);
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
				handle_builtin(command, info);
			}
		}
	return (0);
}

/*
 * Loops through commands list and creates processes accordingly
 *
 * Arguments:
 * - commands - list of commands/processes
 * - info - info structure
 * 
 * Returns:
 * TBD
 */
void	exec(t_list *commands, t_info *info)
{
	int		prev_pipe;
	t_list	*current;
	
	prev_pipe = dup(0);// initialized with any valid fd to avoid error, since first child doesn't need prev_pipe
	current = commands;
	while (current && current->next && !info->exit_flag)
	{
		create_process((t_command *)(current->content), info, &prev_pipe);
		current = current->next;
	}
	if (!info->exit_flag)
		last_process((t_command *)(current->content), info, prev_pipe);

	// TODO: free(commands)
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

void	set_envp(t_info *info)
{
	char	**envp_curr;
	char	**keyval;
	char	*key;
	char	*value;

	envp_curr = info->envp;
	while (*envp_curr)
	{
		keyval = ft_split(*envp_curr, '=');
		key = ft_strdup(keyval[0]);
		if (keyval[1])
			value = ft_strdup(keyval[1]);
		else
			value = NULL;
		free_split(keyval);
		if (!info->envp_list) // if no env set
			info->envp_list = create_envp_node(key, value);
		else
			check_envs(info, key, value);
		envp_curr++;
	}
}

///////////////////////////////////////////////////////////////////////
// For testing
//////////////////////////////////////////////////////////////////////

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
		print_error("malloc error\n", 0);
	}
	info->envp = copy_envp(envp);
	if (getenv("PWD"))
		info->pwd = ft_strdup(getenv("PWD"));
	if (getenv("OLDPWD"))
	 	info->oldpwd = ft_strdup(getenv("OLDPWD"));
	paths = ft_strdup(getenv("PATH"));
	info->path = ft_split(paths, ':');
	free(paths);
	info->envp_list = NULL;
	info->exit_flag = 0;
	info->is_multiple_proc = 0;
	return (info);
}

void	t_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*node;
	t_list	*next_node;

	if (!*lst)
		return ;
	node = *lst;
	while (node)
	{
		del(node->content);
		next_node = node->next;
		free(node);
		node = next_node;
	}
	*lst = NULL;
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
	t_lstclear(&(cmd->args), del_cmd_content);
	free(cmd);
}

int main(int argc, char **argv, char **envp)
{
	t_info *info;

	info = create_info(envp);
	set_envp(info);
	char *cmd1[] = {"export", "aaas=eewrfweno"};//{"echo", "-n", "sdofndf"};
	// char *cmd1[] = {"awk", "END { print NR }", "test.txt"};
	// char *cmd2[] = {"env", "2=22"};
	// char *cmd3[] = {"export", "3=33"};
	t_list *args1, *args2, *args3, *main_list;
	t_command *comm1, *comm2, *comm3;
	//info->envp_list = create_envp_node("aaas", "00");
	args1 = ft_lstnew((void *)cmd1[0]);
	args1->next = ft_lstnew((void *)cmd1[1]);
	//args1->next->next = ft_lstnew((void *)cmd1[2]);
	// args2 = ft_lstnew((void *)cmd2[0]);
	// args2->next = ft_lstnew((void *)cmd2[1]);
	// args2->next->next = ft_lstnew((void *)cmd2[2]);
	// args3 = ft_lstnew((void *)cmd3[0]);
	// args3->next = ft_lstnew((void *)cmd3[1]);
	comm1 = create_command(args1, NULL, NULL, 0);
	// comm2 = create_command(args2, NULL, NULL, 0);
	// comm3 = create_command(args3, NULL, NULL, 0);
	main_list = ft_lstnew((void *)comm1);
	//main_list->next = ft_lstnew((void *)comm2);
	// main_list->next->next = ft_lstnew((void *)comm3);
	exec(main_list, info);
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