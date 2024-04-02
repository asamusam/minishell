/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <asamuilk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/02 15:06:00 by asamuilk         ###   ########.fr       */
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

typedef struct s_token
{
	int				type;
	char			*value;
}	t_token;

typedef struct s_envp
{
	char			*key;
	char			*value;
	char			**pair;
}	t_envp;

typedef struct s_info
{
	char			*reserved_words[7];
	char			**path;
	char			**envp;
	t_list			*envp_list;
	int				envp_flag;
	int				exit_flag;
	int				return_code;
}	t_info;

// envp.c

void	init_envp(char **envp, t_info *minishell);

// signals.c

void	set_signal_handler(void);

// utils.c

int		print_error(char *message, int type);
void	print_token(void *arg);
void	free_token(void *arg);
void	free_split(char **arr);

// envp.c

void	print_envvar(void *arg);
void	free_envvar(void *arg);

// lexer.c

t_list	*lexer(char *line);

// parser.c

t_list	*parser(t_list *tokens, t_info *minishell);

#endif