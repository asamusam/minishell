/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:57:01 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/22 12:29:14 by asamuilk         ###   ########.fr       */
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
 * Zero.
 */
int	print_error(char *message, int type)
{
	if (type == PERROR)
		perror(message);
	else if (type == STDERR)
		ft_putendl_fd(message, STDERR_FILENO);
	return (0);
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

/*
 * Function: free_split
 * ----------------------------
 * Frees all the memory associated with ft_split() return.
 * 
 * Arguments:
 * - arr — array of strings returned by ft_split()
 * 
 * Returns:
 * Nothing.
 */
void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
}
