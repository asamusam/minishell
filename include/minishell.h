/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/09 09:19:41 by mmughedd         ###   ########.fr       */
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

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libft/include/libft.h"

char	*get_envvar(char **envp, char *var);
void	free_split(char **arr);
void	set_signal_handler(void);

// possible structure for each process
typedef struct s_process
{
	char				*input_line;
	struct s_process	*prev;
	struct s_process	*next;
} 	t_process;

//possible structure for redirect

typedef struct s_redirect
{
	char	*file; // name
	int		mode; // O_CREATE,ect
	int		fd; // 0 = stdin / 1 = stdout
	char	*cmd; //command
} t_redirect;

// each node in the list of tokens will 
// contain a pointer to this structure
// (the list type itself is defined in libft)
typedef struct s_token
{
	int				type;
	char			*value;
	int				len;
	struct s_token	*next; //decide if using this or s_tok_list
}	t_token;

// // list of token // or we could add a next to s_token
// typedef struct s_tok_list
// {
// 	t_token		*token;
// 	t_token		*next;
// }	t_tok_list;

// environment variable structure
typedef struct s_envp
{
	char			*key;
	char			*value;
}	t_envp;

/* commented out to silence error

// array of pointers to builtin functions
typedef	(*t_builtin_ptr)(t_list *args, t_info *info);


// main general info structure
typedef struct s_info
{
	t_builtin_ptr	builtins;
	char			*reserved_words[7];
	char			**path;
	char			**envp;
	t_list			envp_list;
	int				envp_flag;
	int				exit_flag;
	int				return_code;
}	t_info;
*/
#endif
