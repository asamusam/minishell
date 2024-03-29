/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/28 20:51:33 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define SYNTAX_ERROR "minishell: syntax error"
# define FAIL 0
# define SUCCESS 1
# include "libft.h"
# include "minishell.h"
# include "unistd.h"
# include "fcntl.h"

typedef struct s_command
{
	t_list	*args;
	int		file_in;
	int		file_out;
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
void	free_command(void *arg);
t_list	*free_commands_return_null(t_list *commands);
t_list	*free_tokens_return_null(t_list *tokens);
t_list	*free_groups_return_null(t_list *groups);
void	print_command(void *arg);
t_list	*get_commands(t_list *groups);

#endif