/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/27 17:29:57 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define SYNTAX_ERROR "minishell: syntax error"
# define FAIL 0
# define SUCCESS 1
# include "libft.h"
# include "minishell.h"

// the executor gets a list of commands, i.e. t_list *commands.
// one command - one part of the pipeline.
// each node in the list of commands will contain a pointer to the structure below.
typedef struct s_command
{
	t_list	*args; // list of arguments (the first argument is a command, rest are args)
	char	*file_in; // file to get the input from
	char	*file_out; // file to write the output to
	int		file_out_flag;	// bit mask for file_out, so to check if it is for truncating, 
				  			// for example, you do "if (flag & O_TRUNC)"
				  			// for file_in it is always O_RDONLY
}	t_command;

void	print_group(void *arg);
void	free_token_list(void *arg);
int		expand(t_list *tokens, t_info *minishell);
int		is_expandable(int type);
char	*get_envp_value(char *key, int len, t_info *minishell);
int		concat_strings(char **str, char *to_join);
int		merge_tokens(t_list **tokens, t_token **token, t_list *merge);
int		free_and_fail(char *str);
void	change_token_value(t_token *token, char *new_value);
int		expand_special(char **dst, char *to_join, int *index, int i);

#endif