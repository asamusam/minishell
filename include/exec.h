/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 12:17:56 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/14 13:52:30 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <fcntl.h>
#include "minishell.h"
#include "parser.h"

typedef struct s_pipe
{
	pid_t	pid;
	int		pipefd[2];
	int		prev_pipe;
	int		orig_stdout;
	int		orig_stdin;
}	t_pipe
;

int		handle_echo(t_list *args);
int		handle_cd(t_list *args, t_info *minishell);
int		handle_pwd(t_info *minishell);
int		update_env(t_list *env_list, char *value);
int		check_envs(t_info *minishell, char *key, char *value);
int		handle_export(t_list *args, t_info *minishell);
int		handle_unset(t_list *args, t_info *minishell);
int		handle_env(t_info *minishell);
int		handle_exit(t_list *args, t_info *minishell);
int		is_buitin(char *cmd);
int		handle_builtin(t_command *command,  t_info *minishell);
int		handle_input(t_command *command, t_info *minishell);
int		print_export(t_info *minishell);
int		find_equal(char *input);
int		check_input(char *input);
int		update_envstr(t_info *minishell);
int		handle_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_last_redirection(t_pipe *pipet, t_command *command);
int		handle_redirections(t_pipe *pipet, t_command *command);
int		handle_bltn_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_blt_redirections(t_pipe *pipet, t_command *command);
int		update_envstr(t_info *minishell);
char	*get_cmd(char **path, char *command);
char	**get_args(t_list *arg_lst);
char	*find_envp_line(char *key, char **envp);
char	**copy_envp(char **envp);
void	free_info(t_info *minishell);
void	del_content(void *content);
void	del_cmd_content(void *content);
void	del_lst_content(void *content);
void	del_env_content(void *content);
void	free_info(t_info *minishell);
void	get_keyval(char *input, char **key, char **value);
void	set_envp(t_info *minishell, char **envp);
void	set_pwds(t_info *minishell);
t_list	*create_envp_node(char *key, char *value);

#endif