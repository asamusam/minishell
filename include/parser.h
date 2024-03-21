/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:00:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/21 11:24:25 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "../libft/include/libft.h"
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

#endif

/*
Input Redirection (<):

< file: Redirects the standard input (stdin) of a command to come from the contents of the specified file.
Output Redirection (>):

> file: Redirects the standard output (stdout) of a command to be written to the specified file. If the file exists, it is overwritten.
>> file: Redirects the standard output (stdout) of a command to be appended to the specified file.
Combining Input and Output Redirection:

< input_file > output_file: Redirects both the standard input (stdin) and standard output (stdout) of a command. The input comes from input_file, and the output goes to output_file.
< input_file command > output_file: Redirects the standard input (stdin) from input_file and the standard output (stdout) of command to output_file.
*/