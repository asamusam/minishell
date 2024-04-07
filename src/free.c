/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:02:42 by asamuilk          #+#    #+#             */
/*   Updated: 2024/04/04 17:02:52 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Frees all the memory associated with the envp structure.
 * 
 * Arguments:
 * - arg — pointer to a structure of type t_envp.
 * 
 * Returns:
 * Nothing.
 */
void	free_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	free(var->key);
	free(var->value);
	free(var);
}
