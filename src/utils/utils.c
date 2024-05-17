/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:57:01 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/10 15:36:03 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Prints given error message.
 *
 * Arguments:
 * - message — error message to print
 * - type — function to use: PERROR for raising perror() 
 * with 'message' as an arg and STDERR for printing 'message' 
 * to the standard error
 * 
 * Returns:
 * One.
 */
int	print_error(char *message, int type)
{
	if (type == PERROR)
		perror(message);
	else if (type == STDERR)
		ft_putendl_fd(message, STDERR_FILENO);
	return (FAIL);
}

/*
 * Prints the contents of the environment variable structure (t_envp).
 *
 * Arguments:
 * - arg — pointer to a structure of type t_envp.
 * 
 * Returns:
 * Nothing.
 */
void	print_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	printf("----------\nkey: %s\nvalue: %s\n", \
			var->key, var->value);
}

/*
 * Prints the contents of a token structure: type, value, length.
 *
 * Arguments:
 * - arg — pointer to the token structure
 * 
 * Returns:
 * Nothing.
 */
void	print_token(void *arg)
{
	t_token	*token;

	token = (t_token *)arg;
	ft_printf("token type: %d\ntoken value: %s\n", token->type, token->value);
}

/*
 * Frees all the memory associated with the t_token structure.
 * 
 * Arguments:
 * - arg — pointer to the token structure
 * 
 * Returns:
 * Nothing.
 */
void	free_token(void *arg)
{
	t_token	*token;

	token = (t_token *)arg;
	free(token->value);
	free(token);
}
