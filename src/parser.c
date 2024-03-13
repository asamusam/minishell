/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:47:54 by mmughedd          #+#    #+#             */
/*   Updated: 2024/03/13 11:44:18 by mmughedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_list *create_node(void *content)
{
	t_list *new_node;
	
	new_node = malloc(sizeof(t_list));
	new_node->content = content;
	new_node->next = NULL;
	return new_node;
}

void	add_node(t_list **head, void *content)
{
	t_list *new_node = NULL;
	
	new_node = create_node(content);
	if (*head == NULL) {
		*head = new_node;
	} else {
		t_list *current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

t_list *create_sub_list(t_list **head)
{
	t_list	*sub_list = NULL;
	t_list	*current;
	
	current = *head;
	while (current && ((t_token *)(current->content))->type != 9)
	{
		//printf("1: %s\n", ((t_token *)(current->content))->value);
		add_node(&sub_list, current->content);
		//printf("2: %s\n", ((t_token *)(current->content))->value);
		current = current->next;
		//printf("3: %s\n", ((t_token *)(current->content))->value);
	}
	if (current)
		*head = current->next;
	return (sub_list);
}

t_list *create_list_of_processes(t_list *head)
{
	t_list	*list_of_lists = NULL;
	t_list	*sub_list;

	while (head)
	{
		sub_list = create_sub_list(&head);
		add_node(&list_of_lists, sub_list);
		head = head->next;
	}
	return (list_of_lists);
}

void test_print(void *token)
{
	if (token)
		printf("%s\n", ((t_token *)token)->value);
}

t_list	*parser(t_list *token)
{
	t_list	*t_processes;

	t_processes = create_list_of_processes(token);
	
	return (t_processes);
}

void *create_token(int type, char *value, int len)
{
	t_token	*token;
	
	token = malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->len = len;
	return ((void *)token);
}

int main()
{
	t_list *first;
	t_list *test;

	char one[] = "$s$name$re"; // word, not extendable type 1
	char two[] = "asdf"; // space, type 2
	char three[] = "$name"; //single var
	char four[] = "xxx$name$name"; //double var
	char five[] = "$names"; // wrong var
	char six[] = "$s$name$re"; // var between wrong vars
	
	
	first = ft_lstnew(create_token(1, one, 5));
	first->next = ft_lstnew(create_token(5, two, 1));
	first->next->next = ft_lstnew(create_token(4, three, 4));
	first->next->next->next = ft_lstnew(create_token(2, four, 4));
	first->next->next->next->next = ft_lstnew(create_token(4, five, 4));
	first->next->next->next->next->next = ft_lstnew(create_token(4, six, 4));
//	ft_lstiter(first, test_print);
	test = parser(first);
	while (test)
	{
		ft_lstiter(test, test_print);
		printf("****************\n");
		test = test->next;
	}
	//free_token(first);
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

//char	*line;
	//char	**extended;
	// int		i;

	// if (!token)
	// 	return (NULL); //empty input
	// line = malloc(sizeof(char) * (token->len + 1));
	// if (!line)
	// 	return (NULL);//malloc error, perror?
	// while (token)
	// {
		// i = 0;
		// if (token->type == 9) //pipe
		// {
		// 		// create process with line command
		// 		// free line
		// 		token = token->next;
		// }
		// else if (token->type <= 4 || token->type == 8) // strings, space, single and double quotes / insource
		// {
		// 	if (token->type == 4 && ft_strchr(token->value, '$')) // is_extendable?
		// 	{
		// 		if (!ft_strnstr(token->value, "$", 1)) // check if first splitted word is env var
		// 			i = 1;
		// 		extended = ft_split(token->value, '$');
		// 		while (extended[i])
		// 			line = join_token(line, getenv(extended[i++]));
		// 		free_split(extended);
		// 	}
		// 	else // no expand
		// 		line = join_token(line, token->value);
		// }
		// else // < or > or >>
		// {
		// 	//
		// }
		// token = token->next;
	// }
	// printf("%s\n", line);
	// return (line);

// 	t_token *create_token(int type, char *value, int len)
// {
// 	t_token *token;

// 	token = malloc(sizeof(t_token));
// 	token->type = type;
// 	token->value = value;
// 	token->len = len;
// 	token->next = NULL;
// 	return (token);
// }

// void	free_token(t_token *token)
// {
// 	t_token *tmp;
// 	while (token)
// 	{
// 		token->type = 0;
// 		token->value = NULL;
// 		token->len = 0;
// 		tmp = token->next;
// 		token->next = NULL;
// 		free(token);
// 		token = tmp;
// 	}	
// }


// char	*join_token(char *line, char *line_to_add)
// {
// 	char	*tmp;

// 	if (!line_to_add)
// 		return(line);
// 	tmp = ft_strjoin(line, line_to_add);
// 	free(line);
// 	return (tmp);
// }
