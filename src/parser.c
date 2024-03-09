/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:47:54 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/09 14:14:12 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	*join_token(char *line, char *line_to_add)
{
	char	*tmp;

	tmp = ft_strjoin(line, line_to_add);
	free(line);
	return (tmp);
}

int	find_env_var(char *line, char *to_extend)
{
	int		len;
	char	*tmp;

	len = 1;
	ft_strlcpy(tmp, to_extend, len);
	while (!getenv(tmp) && to_extend[len])
	{
		free(tmp);
		ft_strlcpy(tmp,	to_extend, ++len);
	}
	line = join_token(line, tmp);
	free(tmp);
	return (len);
}

// when tokenizing using a flag to check for pipes? 
void	parser(t_token *token) // it can be changed to updated t_token
{
	char	*line;
	char	**extended;
	int		i;
	int		len;

	if (!token)
		return ; //empty input
	line = malloc(sizeof(char) * (token->len + 1));
	if (!line)
		return ;//malloc error
	i = 0;
	while (token)
	{
		if (token->type == 9) //pipe
		{
				// create process with line command
				break;
		}
		else if (token->type <= 4)
		{
			if (token->type == 4 && ft_strchr(token->value, '$'))
			{
				extended = ft_split(token->value, '$'); //what if $i is token->value[0]?
				while (extended[i])
				{
					len = find_env_var(line, extended[i++]);
					line = join_token(line, extended[len - 1]);
				}
			}
			else // no expand
				line = join_token(line, token->value);
		}
		token = token->next;
	}
	printf("%s", line);
}


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

int main(int argc, char *argv[])
{
	t_token *first;
	char one[100];
	strcpy(one, argv[1]);
	char two[100];
	strcpy(two, argv[2]);
	char three[100];
	strcpy(three, argv[3]);
	first = create_token(1, one, 5);
	first->next = create_token(2, two, 5);
	first->next->next = create_token(4, three, 4);
	parser(first);
}

// HOW TO TEST
// cc -g parser.c ../libft/libft.a ../include/minishell.h

// name = "John"

// ./a.out "hello" " " "you"		returns "hello you"

// ./a.out "hello" " " "$name"		returns "hello John"

// ./a.out "hello" " " "Mr $name"	returns "hello Mr John"

// ./a.out "hello" " " "Mr $namesas"	returns "hello Mr "
	//it should return "hello   Mr \n"
//TODO: Understand why it expands regardless of type
