/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asamuilk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:03:30 by asamuilk          #+#    #+#             */
/*   Updated: 2024/03/18 16:04:22 by asamuilk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_group(void *arg)
{
	t_list	*group;

	group = (t_list *)arg;
	printf("----GROUP-----\n");
	ft_lstiter(group, print_token);
}

void	free_groups(void *arg)
{
	t_list	*group;

	group = (t_list *)arg;
	ft_lstclear(&group, free);
}
