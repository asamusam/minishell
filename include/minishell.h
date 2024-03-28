/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/28 10:20:09 by mmughedd         ###   ########.fr       */
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
# include "../libft/include/libft.h"

void	set_signal_handler(void);
int		print_error(char *message, int type);
void	print_token(void *arg);
t_list	*lexer(char *line);
void	free_split(char **arr);

// each node in the list of tokens will 
// contain a pointer to this structure
// (the list type itself is defined in libft)
typedef struct s_token
{
	int				type;
	char			*value;
	int				len;
}	t_token;

// environment variable structure
typedef struct s_envp
{
	char			*key;
	char			*value;
}	t_envp;

// array of pointers to builtin functions
//typedef	(*t_builtin_ptr)(t_list *args, t_info *info);

// main general info structure
typedef struct s_info
{
	//t_builtin_ptr	builtins;
	char			**path;
	char			**envp; //TODO:
	t_list			*envp_list;
	int				envp_flag; // TODO:
	int				exit_flag; // TODO:
	int				return_code; //TODO:
}	t_info;

#endif
