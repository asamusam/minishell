/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/23 12:02:42 by asamuilk         ###   ########.fr       */
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
# define FAIL 1
# define SUCCESS 0
# define SYNTAX_ERROR "minishell: syntax error"
# define SYNTAX_FAIL 2

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
	char	**path;
	char	**envp;
	char	*pwd;
	char	*oldpwd;
	char	*home;
	t_list	*envp_list;
	int		exit_flag;
	int		exit_code;
	int		last_prc;
	int		**pipes;
	int		psize;
}	t_info;

// free.c

void	free_split(char **arr);
void	free_envvar(void *arg);
void	free_minishell_info(t_info *minishell);

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

// lexer.c

t_list	*lexer(char *line, int *status);

// parser.c

t_list	*parser(t_list *tokens, t_info *minishell, int *status);

// parser_free.c

void	free_command(void *arg);

// parser_print.c

void	print_command(void *arg);

// exec.c

int		execute(t_list *commands, t_info *minishell);

#endif