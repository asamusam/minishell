/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:48:43 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/04 17:28:12 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include "minishell.h"

// lexer_token_func_spec.c

int		add_pipe(t_list **token_lst, char *line);
int		add_redir_insource(t_list **token_lst, char *line);
int		add_redir_in(t_list **token_lst, char *line);
int		add_redir_append(t_list **token_lst, char *line);
int		add_redir_out(t_list **token_lst, char *line);

// lexer_token_func_word.c

int		add_word(t_list **token_lst, char *line);
int		add_separator(t_list **token_lst, char *line);
int		add_exp_field(t_list **token_lst, char *line);
int		add_field(t_list **token_lst, char *line);

// lexer_utils.c

int		is_word(char c);
t_token	*create_token(int type, char *value, int len);
t_list	*add_token(t_list **token_lst, t_token *token);

#endif