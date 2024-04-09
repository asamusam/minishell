/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/09 14:48:26 by mmughedd         ###   ########.fr       */
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

// parser_cmd.c

t_command	*init_command(void);
int			add_arg(t_list **args, char *value);
int			add_command(t_list **commands, t_command *cmd);
t_command	*get_command(t_list *group);
t_list		*get_commands(t_list *groups);

// parser_expand_utils.c

int			expand_special(char **dst, char *to_join, int *index, int i);
int			free_and_fail(char *str);
void		change_token_value(t_token *token, char *new_value);

// parser_expand.c

int			expand_dollar(char *key, char **dst, t_info *minishell, int *index);
int			expand_token(t_token *token, t_info *minishell);
t_list		*merge_nodes(t_list	*dst, t_list *src);
int			merge_tokens(t_list **tokens, t_token **token, t_list *merge);
int			expand(t_list *tokens, t_info *minishell);

// parser_free.c

void		free_command(void *arg);
t_command	*free_command_return_null(t_command *command);
t_list		*free_commands_return_null(t_list *commands);
t_list		*free_tokens_return_null(t_list *tokens);
t_list		*free_groups_return_null(t_list *groups);

// parser_print.c

void		print_group(void *arg);
void		print_command(void *arg);

// parser_redir.c

int			get_input(char *delimiter, int fd);
int			handle_insource(char *delimiter);
int			handle_redirect(int type, char *file, t_command *cmd);

// parser_utils.c

int			is_expandable(int type);
char		*get_envp_value(char *key, int len, t_info *minishell);
int			concat_strings(char **str, char *to_join);
void		free_token_list(void *arg);

// parser.c

int			check_syntax(t_list *tokens);
t_list		*split_groups(t_list *tokens);
int			expand_groups(t_list *groups, t_info *minishell);
t_list		*parser(t_list *tokens, t_info *minishell);

#endif