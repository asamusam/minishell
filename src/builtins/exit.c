/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:18:10 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/28 11:21:31 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	check_status(char *arg)
{
	int	status;
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i++]))
			return (status = -1);
	}
	status = ft_atoi(arg);
	if (status < 0 || status > 255)
		status = -1;
	return (status);
}

int	handle_exit(t_list *args, t_info *info)
{
	int	status;

	status = 0;
	if (args->next->next)
		print_error("exit: too many args\n", 1);
	if (args->next)
		status = check_status((char *)(args->next)->content);
	if (status == -1)
		print_error("exit status error\n", 1);
	info->exit_flag = 1; // TODO: 
	info->return_code = status;
	return (0);
}