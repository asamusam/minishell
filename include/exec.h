/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 12:17:56 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/09 15:25:55 by mmughedd         ###   ########.fr       */
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
}	t_pipe
;

int		handle_echo(t_list *args);
int		handle_cd(t_list *args, t_info *info);
int		handle_pwd(void);
int		update_env(t_list *env_list, char *value);
int		check_envs(t_info *info, char *key, char *value);
int		handle_export(t_list *args, t_info *info);
int		handle_unset(t_list *args, t_info *info);
int		handle_env(t_info *info);
int		handle_exit(t_list *args, t_info *info);
int		is_buitin(char *cmd);
int		handle_builtin(t_command *command,  t_info *info);
int		handle_input(t_command *command, t_info *info);
int		print_export(t_info *info);
int		find_equal(char *input);
int		check_input(char *input);
int		update_envstr(t_info *info);
int		handle_cmd_process(t_pipe *pipet, t_command *command, t_info *info);
int		handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *info);
int		handle_last_redirection(t_pipe *pipet, t_command *command);
int		handle_redirections(t_pipe *pipet, t_command *command);
int		handle_bltn_process(t_pipe *pipet, t_command *command, t_info *info);
int		handle_blt_redirections(t_pipe *pipet, t_command *command);
char	*get_cmd(char **path, char *command);
char	**get_args(t_list *arg_lst);
char	*find_envp_line(char *key, char **envp);
void	free_info(t_info *info);
void	del_content(void *content);
void	del_cmd_content(void *content);
void	del_lst_content(void *content);
void	del_env_content(void *content);
void	free_info(t_info *info);
void	get_keyval(char *input, char **key, char **value);
t_list	*create_envp_node(char *key, char *value);
t_info	*create_info(t_info *info, char **envp);
void	set_envp(t_info *info, char **envp);
char	**copy_envp(char **envp);
int		update_envstr(t_info *info);
void	set_pwds(t_info *info);

#endif