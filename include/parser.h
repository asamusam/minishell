/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/15 15:33:36 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "libft.h"
# include "minishell.h"

typedef struct s_command
{
	t_list	*args; // list of arguments, where the first argument is a command, and the rest are args
	char	*file_in; // file to get the input from
	char	*file_out; // file to write the output to
	int		file_out_flag;	// bit mask for file_out, so to check if it is for truncating, 
				  			// for example, you do "if (flag & O_TRUNC)"
				  			// for file_in it is always O_RDONLY
}	t_command;

#endif