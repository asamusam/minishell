/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 12:17:56 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/20 00:30:53 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <sys/types.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <stdbool.h>
# include "parser.h"

# define MALLOC_ERROR "minishell: malloc error"
# define PIPE_ERROR "minishell: pipe error"
# define FORK_ERROR "minishell: fork error"
# define CLOSE_ERROR "minishell: close error"
# define CD_ERROR "minishell: cd"
# define DUP_ERROR "minishell: dup error"
# define DUP2_ERROR "minishell: dup2 error"
# define DIR_ERROR "minishell: cd: No such file or directory"
# define HOME_ERROR "minishell: cd: HOME not set"
# define ARGS_ERROR "minishell: cd: too many arguments"
# define NUM_ARG_ERROR "minishell: exit: numeric argument required"
# define EX_ARGS_ERROR "minishell: exit: too many args"
# define EXPORT_ERROR "minishell: export: not a valid identifier"
# define PIPET_ERROR "minishell: pipet error"
# define WAIT_ERROR "minishell: wait error"
# define COMMAND_ERROR "minishell: command error: command not found"
# define EXECVE_ERROR "minishell: execve error"

int		handle_echo(t_list *args);
int		handle_cd(t_list *args, t_info *minishell);
int		handle_pwd(t_info *minishell);
int		check_envs(t_info *minishell, char *key, char *value);
int		handle_export(t_list *args, t_info *minishell);
int		handle_unset(t_list *args, t_info *minishell);
int		handle_env(t_info *minishell);
int		handle_exit(t_list *args, t_info *minishell);
int		is_builtin(char *cmd);
int		handle_builtin(t_command *command, t_info *minishell);
int		handle_input(t_command *command, t_info *minishell);
int		print_export(t_info *minishell);
int		find_equal(char *input);
int		check_input(char *input);
int		update_envstr(t_info *minishell);
int		update_envstr(t_info *minishell);
int		update_envp_pwd(t_info *minishell, char *newpwd);
int		update_envp(t_list *envp_list, char *value);
int		get_keyval(char *input, char **key, char **value);
char	*get_cmd(char **path, char *command);
char	**get_args(t_list *arg_lst);
char	**copy_envp(char **envp);
void	del_content(void *content);
void	set_envp(t_info *minishell, char **envp);
void	set_pwds(t_info *minishell);
t_list	*create_envp_node(char *key, char *value);

int		redir_stdin(int	*original_stdin, int fd, int pipe);
int		redir_stdout(int *original_stdout, int fd, int pipe);
int		restore_stdin(int original_stdin, int fd, int pipe);
int		restore_stdout(int original_stdout, int fd, int pipe);
int		close_pipes(int **pipes, int i, int i_max);
void	free_pipes(int **pipes, int size);
int		free_pipes_return_fail(int **pipes, int size, char *error_type);
int		run_command(t_command *command, t_info *minishell, int in, int out);
void	waiting_signal_handler(int signal);
void	wait_for_children(t_info *minishell);

#endif