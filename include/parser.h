/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/20 16:35:36 by asamuilk         ###   ########.fr       */
=======
/*   Updated: 2024/03/22 12:09:08 by asamuilk         ###   ########.fr       */
>>>>>>> 19f3307 (added comments for some functions in parser)
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define SYNTAX_ERROR "minishell: syntax error"
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
<<<<<<< HEAD
void	free_groups(void *arg);
void	expand(void *arg);
=======
void	free_token_list(void *arg);
>>>>>>> 19f3307 (added comments for some functions in parser)

#endif