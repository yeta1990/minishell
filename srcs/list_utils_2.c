/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:09:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/03 23:55:11 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	t_files_size(t_files **full_strings)
{
	t_files	*aux;
	int		size;

	size = 0;
	aux = *full_strings;
	while (aux)
	{
		aux = aux->next;
		size++;
	}
	return (size);
}

char	**set_empty_double_char(t_files **full_strings)
{
	char	**s;

	s = malloc(sizeof(char *) * (2));
	s[0] = ft_strdup("");
	s[1] = 0;
	free_files(*full_strings);
	free(full_strings);
	return (s);
}

char	**from_list_to_double_char_sep(t_sep **w)
{
	char	**s;
	t_files	*aux;
	int		size;

	size = t_files_size((*w)->separated_pipes);
	aux = *(*w)->separated_pipes;
	if (size == 0)
		return (set_empty_double_char((*w)->separated_pipes));
	s = malloc(sizeof(char *) * (size + 1));
	size = 0;
	while (aux)
	{
		s[size] = ft_strdup(aux->name);
		aux = aux->next;
		size++;
	}
	s[size] = 0;
	free_files(*(*w)->separated_pipes);
	free((*w)->separated_pipes);
	free(*w);
	return (s);
}

char	**from_list_to_double_char(t_files **full_strings)
{
	char	**s;
	t_files	*aux;
	int		size;

	size = t_files_size(full_strings);
	aux = *full_strings;
	if (size == 0)
		return (set_empty_double_char(full_strings));
	s = malloc(sizeof(char *) * (size + 1));
	size = 0;
	while (aux)
	{
		s[size] = ft_strdup(aux->name);
		aux = aux->next;
		size++;
	}
	s[size] = 0;
	free_files(*full_strings);
	free(full_strings);
	return (s);
}
