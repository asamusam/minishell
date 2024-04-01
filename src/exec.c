/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/25 10:03:39 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/exec.h"
#include "../include/minishell.h"

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

	if ((pid = fork()) == -1)
	{
		print_error("Fork error\n", 0); // TODO: check error
		return (1);
	}
	if (pid == 0)
	{
		dup2(prev_pipe, STDIN_FILENO); // TODO: check for redirection
		close(prev_pipe);
		if (!is_buitin(command->args->content))
			handle_input(command, info);
	}
	else
	{
		close(prev_pipe);
		waitpid(pid, NULL, 0);
		if (is_buitin(command->args->content))
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
	if (pid == 0) // child process
	{
		close (pipefd[0]);
		dup2 (pipefd[1], STDOUT_FILENO);
		close (pipefd[1]);
		dup2 (*prev_pipe, STDIN_FILENO);
		close (*prev_pipe);
		if (!is_buitin(command->args->content))
			handle_input(command, info);
	}
	else
	{
		close(pipefd[1]);
		close(*prev_pipe);
		*prev_pipe = pipefd[0];
		waitpid(pid, NULL, 0);
		if (is_buitin(command->args->content))
			handle_builtin(command, info);
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
	t_list *p; // TODO: to remove after testing
	int i = 0;
	prev_pipe = dup(0);// initialized with any valid fd to avoid error, since first child doesn't need prev_pipe
	current = commands;
	while (current && current->next)
	{
		p = info->envp_list;
		//printf("key: %s\n", ((t_envp *)p->content)->key);
		while(p)
		{
			printf("%d: key: %s, value: %s\n", i++, ((t_envp *)p->content)->key, ((t_envp *)p->content)->value);
			p = p->next;
		}
		printf("*******************\n");
		create_process((t_command *)(current->content), info, &prev_pipe);
		current = current->next;
		printf("*******************\n");
		p = info->envp_list;
		//printf("key: %s\n", ((t_envp *)p->content)->key);
		while(p)
		{
			printf("%d: key: %s, value: %s\n", i++, ((t_envp *)p->content)->key, ((t_envp *)p->content)->value);
			p = p->next;
		}
		printf("*******************\n");
	}
	last_process((t_command *)(current->content), info, prev_pipe);
	printf("*******************\n");
	p = info->envp_list;
	while(p)
	{
		printf("%d: key: %s, value: %s\n", i, ((t_envp *)p->content)->key, ((t_envp *)p->content)->value);
		p = p->next;
	}
	printf("*******************\n");
	//printf("key: %s\n", ((t_envp *)info->envp_list->content)->key);
	// TODO: free(commands)

}

///////////////////////////////////////////////////////////////////////
// For testing
//////////////////////////////////////////////////////////////////////

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

int main(int argc, char **argv, char **envp)
{
	//char *cmd1[] = {"cd", "test"};//{"echo", "-n", "sdofndf"};
	char *cmd1[] = {"export", "sad1=dsd"};
	char *cmd2[] = {"unset", "sad1", "vvvvv"};
	char *cmd3[] = {"export", "sad=dasdsasadsd"};
	char a[] = "sad";
	char b[] = "efse";
	char *path = "/home/mmughedd/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin";
	char **pathss;
	pathss = ft_split(path, ':');
	t_list *args1, *args2, *args3, *main_list;
	t_command *comm1, *comm2, *comm3;
	t_info *info;
	info = malloc(sizeof(t_info));
	info->envp = envp;
	info->path = pathss;
	info->envp_list = create_envp_node("vvvvv", "sfndl");
	args1 = ft_lstnew((void *)cmd1[0]);
	args1->next = ft_lstnew((void *)cmd1[1]);
	//args1->next->next = ft_lstnew((void *)cmd1[2]);
	args2 = ft_lstnew((void *)cmd2[0]);
	args2->next = ft_lstnew((void *)cmd2[1]);
	args2->next->next = ft_lstnew((void *)cmd2[2]);
	//args3 = ft_lstnew((void *)cmd3[0]);
	//args3->next = ft_lstnew((void *)cmd3[1]);
	comm1 = create_command(args1, NULL, NULL, 0);
	comm2 = create_command(args2, NULL, NULL, 0);
	//comm3 = create_command(args3, NULL, NULL, 0);
	main_list = ft_lstnew((void *)comm1);
	main_list->next = ft_lstnew((void *)comm2);
	//main_list->next->next = ft_lstnew((void *)comm3);
	exec(main_list, info);
	// char s[] = "Hello\n";
	// char y[] = "Helloy\n";
	// printf("%d\n", ft_strcmp(s, y));
	// printf("%d\n", ft_strncmp(s, y, 5));

}

// gdb
// set follow-fork-mode child: gdb will automatically switch to debugging the child process when a fork() call is encountered.
// set follow-fork-mode parent: gdb will stay with the parent process and not switch to the child process.

// Valgrind with pipes and forks
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./a.out

// compiling
// cc -g utils.c builtins.c builtins_utils.c exec.c exec_utils.c ../libft/libft.a