/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 12:17:56 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/18 15:13:18 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <sys/types.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <fcntl.h>
# include "minishell.h"
# include "parser.h"

# define MALLOC_ERROR "minishell: malloc error"
# define PIPE_ERROR "minishell: pipe error"
# define FORK_ERROR "minishell: fork error"
# define CLOSE_ERROR "minishell: close error"
# define CD_ERROR "minishell: cd"
# define DUP_ERROR "minishell: dup error"
# define DUP2_ERROR "minishell: dup2 error"
# define DIR_ERROR "minishell: cd: No such file or directory"
# define HOME_ERROR "minishell: cd: HOME not set\n"
# define ARGS_ERROR "minishell: cd: too many arguments\n"
# define NUM_ARG_ERROR "minishell: exit: numeric argument required\n"
# define EX_ARGS_ERROR "minishell: exit: too many args\n"
# define EXPORT_ERROR "minishell: export: not a valid identifier\n"
# define PIPET_ERROR "minishell: pipet error\n"
# define WAIT_ERROR "minishell: wait error"
# define COMMAND_ERROR "minishell: command error"
# define EXECVE_ERROR "minishell: execve error"

typedef struct s_pipe
{
	pid_t	pid;
	int		pipefd[2];
	int		prev_pipe;
	int		orig_stdout;
	int		orig_stdin;
}	t_pipe;

int		handle_echo(t_list *args);
int		handle_cd(t_list *args, t_info *minishell);
int		handle_pwd(t_info *minishell);
int		update_env(t_list *env_list, char *value);
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
int		handle_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_lst_cmd_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_last_redirection(t_pipe *pipet, t_command *command);
int		handle_redirections(t_pipe *pipet, t_command *command);
int		handle_bltn_process(t_pipe *pipet, t_command *command, t_info *minishell);
int		handle_blt_redirections(t_pipe *pipet, t_command *command);
int		update_envstr(t_info *minishell);
int		update_envp_pwd(t_info *minishell, char *newpwd);
int		update_envp(t_list *envp_list, char *value);
int		handle_parent(t_pipe *pipet);
int		handle_last_parent(t_pipe *pipet);
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

// new_exec

int		redir_stdin(int	*original_stdin, int fd);
int		redir_stdout(int *original_stdout, int fd);
int		restore_stdin(int original_stdin, int fd);
int		restore_stdout(int original_stdout, int fd);
int		close_pipes(int **pipes, int i, int i_max);
void	free_pipes(int **pipes, int size);
int		free_pipes_return_fail(int **pipes, int size, char *error_type);
int		run_command(t_command *command, t_info *minishell, int in, int out);


#endif