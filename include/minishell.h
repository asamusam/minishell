/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/09 14:42:18 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// token types
# define WORD 1
# define SEPARATOR 2
# define FIELD 3
# define EXP_FIELD 4
# define REDIRECT_OUT 5
# define REDIRECT_IN 6
# define REDIRECT_APPEND 7
# define REDIRECT_INSOURCE 8
# define PIPE 9

// error handling
# define PERROR 0
# define STDERR 1

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"

extern volatile sig_atomic_t	g_signal;

typedef struct s_token
{
	int				type;
	char			*value;
}	t_token;

typedef struct s_envp
{
	char			*key;
	char			*value;
}	t_envp;

typedef struct s_info
{
	//t_builtin_ptr	builtins;
	char			**path;
	char			**envp;
	char			*pwd;
	char			*oldpwd;
	char			*home;
	t_list			*envp_list;
	int				envp_flag; // TODO:
	int				exit_flag;
	int				is_multiple_proc;
	int				exit_code;
}	t_info;

// free.c

void	free_envvar(void *arg);

// init.c

void	create_envp_list(char **envp, t_info *minishell);
void	init(char **envp, t_info *minishell);

// signals.c

void	signal_handler(int signal);
void	set_signal_handler(void);

// utils.c

int		print_error(char *message, int type);
void	print_envvar(void *arg);
void	print_token(void *arg);
void	free_token(void *arg);
void	free_split(char **arr);

// lexer.c

t_list	*lexer(char *line);

// parser.c

t_list	*parser(t_list *tokens, t_info *minishell);

// parser_free.c

void	free_command(void *arg);

// parser_print.c

void	print_command(void *arg);

#endif