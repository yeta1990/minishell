/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 16:01:54 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/04 10:05:13 by albgarci         ###   ########.fr       */
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

size_t	get_double_quote_cut(char *s)
{
	size_t	l;
	char	*aux;

	aux = s + 1;
	l = 0;
	if (ft_strchr(aux, '"') && *(ft_strchr(aux, '"') + 1) == ' ')
		l = (ft_strchr(aux, '"') - aux) + 2;
	else if (ft_strchr(aux, '"'))
		l = (ft_strchr(aux, '"') - aux) + 2;
	while (s && s[l] && s[l] != ' ')
		l++;
	return (l);
}

size_t	get_simple_quote_cut(char *s)
{
	size_t	l;
	char	*aux;

	aux = s + 1;
	l = 0;
	if (ft_strchr(aux, '\'') && *(ft_strchr(aux, '\'') + 1) == ' ')
		l = (ft_strchr(aux, '\'') - aux) + 2;
	else if (ft_strchr(aux, '\''))
		l = (ft_strchr(aux, '\'') - aux) + 2;
	while (s && s[l] && s[l] != ' ')
		l++;
	return (l);
}

size_t	std_space_get_cut_space(char *s1)
{
	size_t	l;

	l = 0;
	if (s1 && *s1 == '"')
		return (get_double_quote_cut(s1));
	else if (s1 && *s1 == '\'')
		return (get_simple_quote_cut(s1));
	else
		while (s1 && s1[l] && s1[l] != ' ' && s1[l] != '<' && s1[l] != '>'
			&& s1[l] != 9)
			l++;
	return (l);
}

void	init_checker(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 1);
		exit (0);
	}
	argv += 0;
}
