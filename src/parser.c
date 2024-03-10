/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:47:54 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/10 10:44:23 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*join_token(char *line, char *line_to_add)
{
	char	*tmp;

	if (!line_to_add)
		return(line);
	tmp = ft_strjoin(line, line_to_add);
	free(line);
	return (tmp);
}

char	*parser(t_token *token)
{
	char	*line;
	char	**extended;
	int		i;

	if (!token)
		return (NULL); //empty input
	line = malloc(sizeof(char) * (token->len + 1));
	if (!line)
	{
		return (NULL);//malloc error, perror?
	}
	while (token)
	{
		i = 0;
		if (token->type == 9) //pipe
		{
				// create process with line command
				break;
		}
		else if (token->type <= 4 || token->type == 8) // strings, space, single and double quotes / insource
		{
			if (token->type == 4 && ft_strchr(token->value, '$')) // is_extendable?
			{
				if (!ft_strnstr(token->value, "$", 1)) // check if first splitted word is env var
					i = 1;
				extended = ft_split(token->value, '$');
				while (extended[i])
					line = join_token(line, getenv(extended[i++]));
				free_split(extended);
			}
			else // no expand
				line = join_token(line, token->value);
		}
		token = token->next;
	}
	printf("%s\n", line);
	return (line);
}

// for testing
t_token *create_token(int type, char *value, int len)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->len = len;
	token->next = NULL;
	return (token);
}

void	free_token(t_token *token)
{
	t_token *tmp;
	while (token)
	{
		token->type = 0;
		token->value = NULL;
		token->len = 0;
		tmp = token->next;
		token->next = NULL;
		free(token);
		token = tmp;
	}	
}

int main()
{
	t_token *first;

	char one[] = "$s$name$re"; // word, not extendable type 1
	char two[] = " "; // space, type 2
	char three[] = "$name"; //single var
	char four[] = "  xxx$name$name"; //double var
	char five[] = "$names"; // wrong var
	char six[] = "$s$name$re"; // var between wrong vars
	
	first = create_token(1, one, 5);
	first->next = create_token(2, two, 1);
	first->next->next = create_token(4, three, 4);
	first->next->next->next = create_token(4, four, 4);
	first->next->next->next->next = create_token(4, five, 4);
	first->next->next->next->next->next = create_token(4, six, 4);
	parser(first);
	free_token(first);
}

// HOW TO TEST
// cc -g parser.c ../libft/libft.a ../include/minishell.h
// name = "John" ./a.out

// to set env var in gdb
// set environment name="john"

// no longer needed
// int	find_env_var(char *line, char *to_extend)
// {
// 	int		len;
// 	char	*tmp;

// 	len = 1;
// 	ft_strlcpy(tmp, to_extend, len);
// 	while (!getenv(tmp) && to_extend[len])
// 	{
// 		free(tmp);
// 		ft_strlcpy(tmp,	to_extend, ++len);
// 	}
// 	line = join_token(line, tmp);
// 	free(tmp);
// 	return (len);
// }
