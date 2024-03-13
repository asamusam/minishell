 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:18:06 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/09 06:33:26 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
 * Function: get_envvar
 * ----------------------------
 * Allocates with malloc() and returns the value of 
 * a specified environment variable.
 * 
 * Arguments:
 * - envp - environment variables list;
 * - var - environment variable to find;
 * 
 * Returns:
 * A pointer to the string containing the value of
 * the variable specified in var. NULL if var is not found 
 * or there is no value assigned to it.
 */
char	*get_envvar(char **envp, char *var)
{
	char	*res;

	while (*envp)
	{
		res = ft_strnstr(*envp, var, ft_strlen(*envp));
		if (res && res == *envp)
			return (ft_strdup(ft_strchr(res, '=') + 1));
		envp ++;
	}
	return (NULL);
}

/*
 * Function: free_split
 * ----------------------------
 * Frees all the memory associated with ft_split() return.
 * 
 * Arguments:
 * - arr - array of strings returned by ft_split().
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
