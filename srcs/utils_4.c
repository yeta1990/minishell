/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 16:01:54 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 16:04:19 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	a;

	a = 0;
	while ((*s1 != '\0') || (*s2 != '\0'))
	{
		if (*s1 != *s2)
		{
			a = *s1 - *s2;
			break ;
		}
		else
		{
			a = 0;
		}
		s1 ++;
		s2 ++;
	}
	return (a);
}

int	std_space_get_cut_space(char *s1)
{
	size_t	l;
	char	*aux;

	l = 0;
	if (s1 && *s1 == '"')
	{
		aux = s1 + 1;
		if (ft_strchr(aux, '"') && *(ft_strchr(aux, '"') + 1) == ' ')
			l = (ft_strchr(aux, '"') - aux) + 2;
		else
			while (s1 && s1[l] && s1[l] != ' ' && s1[l] != '<' && s1[l] != '>')
				l++;
	}
	else
		while (s1 && s1[l] && s1[l] != ' ' && s1[l] != '<' && s1[l] != '>')
			l++;
	return (l);
}
