/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 19:55:40 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/18 20:13:11 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	printf("----------\nkey: %s\nvalue: %s\npair: %s\n", \
			var->key, var->value, *var->pair);
}

void	free_envvar(void *arg)
{
	t_envp	*var;

	var = (t_envp *)arg;
	free(var->key);
	free(var->value);
}
