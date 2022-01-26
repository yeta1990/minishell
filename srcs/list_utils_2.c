/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:09:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/26 18:21:12 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**from_list_to_double_char(t_files **full_strings)
{
	char	**s;
	t_files	*aux;
	int		size;

	size = 0;
	aux = *full_strings;
	s = 0;
	while (aux)
	{
		aux = aux->next;
		size++;
	}
	if (size == 0)
	{
		s = malloc(sizeof(char *) * (2));
		s[0] = ft_strdup("");
		s[1] = 0;
		free_files(*full_strings);
		free(full_strings);
		return (s);
	}
	s = malloc(sizeof(char *) * (size + 1));
	aux = *full_strings;
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
