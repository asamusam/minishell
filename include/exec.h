/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 12:17:56 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/24 13:55:32 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <stdbool.h>
#include "minishell.h"
#include "parser.h"

int	handle_echo(t_list *args);
int handle_cd(t_list *args, t_info *info);
int	handle_pwd(void);
int	update_env(t_list *env_list, char *key, char *value);
int	check_envs(t_info *info, char *key, char *value);
t_list	*create_envp_node(char *key, char *value);
int	handle_export(t_list *args, t_info *info);
int	handle_unset(t_list *args, t_info *info);
int	handle_env(t_info *info);
int	handle_exit(t_list *args);
int	is_buitin(char *cmd);
int	handle_builtin(t_command *command,  t_info *info);
int	handle_input(t_command *command, t_info *info);
char	*get_command(char **path, char *command);
char	**get_args(t_list *arg_lst);
char	*find_envp_line(char *key, char **envp);
int	ft_strcmp(char *s1, char *s2);
t_list *create_envp_node(char *key, char *value);
#endif