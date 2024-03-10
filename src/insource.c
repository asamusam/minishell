/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insource.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 10:45:49 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/10 11:53:11 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*insource(char *input)//arg depends on how tokenize, for now i take the input string, we can modify it easily
{
	// note, delimiter is recognized after '<<' with white space;
	char	*delimeter;
	int		i;
	int		j;
	//the string will start after '<< '
	if (!input)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i] && input[i] != ' ')
		i++;
	delimeter = malloc(sizeof(char) * (i + 1));
	if (!delimeter || input[i] != ' ')
		return (NULL); // no delimeter TODO: check if it returns an empty string or throws an error
	ft_strlcpy(delimeter, input, ++i);
	while (input[i + j])
	{
		if (!ft_strncmp(&input[i + j], delimeter, i))
			return (ft_substr(input, i, j));
		j++;
	}
	return (NULL); // open input if no delimeter found TODO: make sure it's the correct case
}

int main(int argc, char *argv[])
{
	char *ins;

	ins = insource(argv[1]);
	if (ins != NULL)
		printf("%s\n", ins);
}

// cc -g insource.c ../libft/libft.a ../include/minishell.h
// ./a.out "END helloda \n dsfoioj \n dfiojoiEND"
