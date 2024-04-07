/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:45:24 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/04 12:46:32 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	del_cmd_content(void *content)
{
	//free(content); //if allocated
	content = NULL;
}

void	del_lst_content(void *content)
{
	t_command	*cmd;

	cmd = (t_command *)content;
	//free(cmd->file_in); 
	//free(cmd->file_out);
	ft_lstclear(&(cmd->args), del_cmd_content);
	free(cmd);
}

void	del_env_content(void *content)
{
	t_envp	*envp;
	
	envp = (t_envp *)content;
	if (!envp)
		return ;
	free(envp->key);
	free(envp->value);
	free(content);
}

void	free_info(t_info *info)
{
	free_split(info->path);
	free_split(info->envp);
	ft_lstclear(&(info->envp_list), &del_env_content);
	free(info);
}