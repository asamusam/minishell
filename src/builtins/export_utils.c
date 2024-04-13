/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:32:21 by mmughedd          #+#    #+#             */
/*   Updated: 2024/04/13 15:01:54 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
 * Prints all envp variables with export cmd format
 *
 * Arguments:
 * - info - info sturcture
 *
 * Returns:
 * Zero
 */
int	print_export(t_info *info)
{
	t_list	*current;
	t_envp	*envp;

	current = info->envp_list;
	while (current)
	{
		envp = (t_envp *)current->content;
		ft_printf("declare -x %s=%s\n", envp->key, envp->value);
		current = current->next;
	}
	return (SUCCESS);
}

/*
 * Looks for '=' location in input
 *
 * Arguments:
 * - input - input in form 'key=value'
 *
 * Returns:
 * Index if found, -1 otherwise
 */
int	find_equal(char *input)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(input);
	while (input[i] && i < len)
	{
		if (input[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/*
 * Get key and value values
 *
 * Arguments:
 * - input - input in form 'key=value'
 * - key - envp key
 * - value - envp value
 *
 * Returns:
 * Status
 */
void	get_keyval(char *input, char **key, char **value)
{
	int	equal_len;
	int	len;

	equal_len = find_equal(input);
	len = ft_strlen(input);
	if (equal_len == len - 1)
	{
		*key = ft_substr(input, 0, len - 1);
		*value = ft_strdup("");
	}
	else
	{
		*key = ft_substr(input, 0, equal_len);
		*value = ft_substr(input, equal_len + 1, len - equal_len);
	}
}

/*
 * Handles export builtin command
 *
 * Arguments:
 * - input - check if key input is valid
 *
 * Returns:
 * 0 if valid, 1 if not
 */
int	check_input(char *input)
{
	int	i;
	int	equal_len;

	i = 0;
	equal_len = find_equal(input);
	if (equal_len == -1)
		equal_len = ft_strlen(input);
	if (equal_len == 0 || (!ft_isalpha(input[i]) && input[i] != '_'))
		return (0);
	while (input[++i] && i < equal_len)
	{
		if (!ft_isalnum(input[i]) && input[i++] != '_')
			return (SUCCESS);
	}
	return (FAIL);
}
