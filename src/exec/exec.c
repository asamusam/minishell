/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:04:55 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/09 14:13:17 by mmughedd         ###   ########.fr       */
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
		handle_last_redirection(pipet, command);
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

	status = 0;
	if (!commands || !commands->content)
		return (status);
	pipet = malloc(sizeof(t_pipe));
	if (!pipet)
		return (print_error("malloc error\n", 0));
	pipet->prev_pipe = dup(0);
	current = commands;
	if (current && current->next)
			info->is_multiple_proc = 1;
	while (current && current->next && !info->exit_code)
	{
		status = create_process((t_command *)(current->content), info, pipet);
		current = current->next;
	}
	if (!info->exit_code)
		status = last_process((t_command *)(current->content), info, pipet);
	free(pipet);
	return (status);
}



///////////////////////////////////////////////////////////////////////
// For testing/////////////////////
///////////////////////////////////////////////////////////////////////

// void printenvv(t_list *lst)
// {
// 	t_list *list;
// 	list = lst;
// 	while (list)
// 	{
// 		printf("%s=%s\n", ((t_envp *)list->content)->key, ((t_envp *)list->content)->value);
// 		list = list->next;
// 	}
// }

// void printenvst(char **envp)
// {
// 	int i = 0;
// 	while (envp[i])
// 		printf("%s\n", envp[i++]);
// }

// t_command *create_command(t_list *args, int in, int out)
// {
// 	t_command *command;

// 	command = malloc(sizeof(t_command));
// 	command->args = args;
// 	command->file_in = in;
// 	command->file_out = out;
// 	return (command);
// }

// int main(int argc, char **argv, char **envp)
// {
// 	t_info *info;

// 	info = create_info(envp);
// 	set_envp(info, envp);
// 	set_pwds(info);
// 	char *cmd1[] = {"ls", "-l", "test.txt"};//{"export", "aaas=eewrfweno"};//
// 	char *cmd2[] = {"cat", "-c"};
// 	//char *cmd3[] = {"export", "3=33"};
// 	t_list *args1, *args2, *args3, *main_list, *mainlist2;
// 	main_list = NULL;
// 	mainlist2 = NULL;
// 	t_command *comm1, *comm2, *comm3;
// 	//info->envp_list = create_envp_node("aaas", "00");
// 	args1 = ft_lstnew((void *)cmd1[0]);
// 	args1->next = ft_lstnew((void *)cmd1[1]);
// 	//args1->next->next = ft_lstnew((void *)cmd1[2]);
// 	args2 = ft_lstnew((void *)cmd2[0]);
// 	//args2->next = ft_lstnew((void *)cmd2[1]);
// 	// args2->next->next = ft_lstnew((void *)cmd2[2]);
// 	//args3 = ft_lstnew((void *)cmd3[0]);
// 	//args3->next = ft_lstnew((void *)cmd3[1]);
// 	int fd1, fd2, fd3;
// 	fd1 = open("/home/mmughedd/minishell/src/test1.txt", O_RDONLY);
// 	fd2 = open("/home/mmughedd/minishell/src/test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 	fd3 = open("/home/mmughedd/minishell/src/test3.txt", O_WRONLY | O_CREAT | O_APPEND, 0664);
// 	comm1 = create_command(args1, -1, fd3);
// 	comm2 = create_command(args2, -1, -1);
// 	//comm3 = create_command(args3, NULL, NULL, 0);
// 	main_list = ft_lstnew((void *)comm1);
// 	//mainlist2 = ft_lstnew((void *)comm2);
// 	main_list->next = ft_lstnew((void *)comm2);
// 	//main_list->next->next = ft_lstnew((void *)comm3);
// 	exec(main_list, info);
// 	exec(mainlist2, info);
// 	//printenvv(info->envp_list);
// 	//printenvst(info->envp);
// 	free_info(info);
// 	ft_lstclear(&(main_list), &del_lst_content);
// 	//ft_lstclear(&(mainlist2), &del_lst_content);
// }
/*
// gdb
// set follow-fork-mode child: gdb will automatically switch to debugging the child process when a fork() call is encountered.
// set follow-fork-mode parent: gdb will stay with the parent process and not switch to the child process.

// Valgrind with pipes and forks

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./a.out

// // compiling

cc -g ./utils.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/export_utils.c builtins/pwd.c builtins/unset.c exec/exec.c exec/exec_bltin_utils.c exec/redirect.c exec/exec_cmd_utils.c info/info.c free/free.c ../libft/libft.a -I ../include/

// */