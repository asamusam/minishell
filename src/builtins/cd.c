/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:34 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/28 11:23:04 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char *get_cdpath(t_list *envp)
{
	char *cdpath;
	t_list *current;

	current = envp;
	cdpath = NULL;
	while (current)
	{
		if (!ft_strcmp((char *)current->content, "CDPATH"))
			cdpath = (char *)current->content;
		current = current->next;
	}
	return (cdpath);
}

int handle_cd(t_list *args, t_info *info)
{
	char *dir;
	char *cdpath;
	char *dirpath;
	char *tmp;
	char **cdpaths;
	t_list *current;

	if (args->next && args->next->next)
	{
		print_error("bash: cd: too many arguments\n", 1); // only one or two args  TODO: exit
		exit(1);
	}
	else if (!args->next) // no directory provided
	{
		dir = getenv("HOME");
		if (dir == NULL) // 1. If no directory operand is given and the HOME environment variable is empty or undefined, the default behavior is implementation-defined and no further steps shall be taken.
		{
			print_error("HOME environment variable not set.\n", 1);
			return (1);
		}
		chdir(dir);
		exit(0);
	}
	else
	{
		current = args->next;
		dir = (char *)current->content;
		if (dir[0] == '/') // absolute path TODO: what about'~'?
		{
			if (chdir(dir) == -1)
			{
				print_error("0 - chdir error\n", 1);
				return (1);
			}
			// printf("Changed to %s\n", dir);
			exit(0);
		}
		else
		{
			cdpath = get_cdpath(info->envp_list);
			printf("%s", cdpath);
			if (!cdpath) //(!(cdpath = getenv("CDPATH")))
			{
				if (chdir(dir) == -1)
				{
					print_error("1 -chdir error\n", 1);
					return (1);
				}
				printf("1 - Changed to %s\n", dir);
				exit(0);
			}
			else
			{ //TODO: to test
				cdpaths = ft_split(cdpath, ':');
				while (*cdpaths)
				{
					tmp = ft_strjoin(*cdpaths, "/");
					dirpath = ft_strjoin(tmp, dir);
					free(tmp);
					if (!access(dirpath, F_OK))
					{
						free_split(cdpaths);
						if (chdir(dirpath) == -1)
						{
							print_error("2 - chdir error\n", 1);
							return (1);
						}
						printf("2 - Changed to %s\n", dir);
						exit(0);
					}
					cdpaths++;
				}
				free_split(cdpaths);
			}
		}
	}
	return (0);
}