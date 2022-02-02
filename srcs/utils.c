/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/02/01 13:05:12 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nwords_2(char const *s, char c)
{
	int	nwords;

	nwords = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		if (*s == c)
		{
			nwords++;
			while (*s == c)
				s++;
		}
		else
			s++;
	}
	nwords++;
	return (nwords);
}

static void	save_words(char **ptr, char *str, char c, int nwords)
{
	int		i;
	char	*aux;

	i = 0;
	if ((nwords > 0) && *str)
	{
		while (i < (nwords - 1))
		{
			aux = ft_strchr(str, c);
			ptr[i++] = ft_substr(str, 0, aux - str);
			while (*aux == c)
				aux++;
			str = aux;
		}
		ptr[i++] = ft_strdup(str);
		ptr[i] = 0;
	}
}

char	**ft_split(char const *s, char c)
{
	int		nwords;
	char	**ptr;
	char	*str;
	char	d[2];

	d[0] = c;
	d[1] = '\0';
	if (s)
	{
		str = ft_strtrim(s, d);
		if (str)
		{
			nwords = get_nwords_2(str, c);
			ptr = ft_calloc((nwords + 1), sizeof(char *));
			if (ptr)
				save_words(ptr, str, c, nwords);
			free(str);
			return (ptr);
		}
	}
	return (0);
}

char	*ft_strdup_space(const char *s1, int *size, int cmd)
{
	size_t	i;
	size_t	l;
	char	*ptr;

	i = 0;
	l = 0;
	if (cmd == 0)
		l = std_space_get_cut_position((char *) s1);
	else if (cmd == 1)
		l = std_space_get_cut_space((char *) s1);
	ptr = malloc(sizeof(char) * (l + 1));
	if (ptr)
	{
		*size += l;
		while (l-- > 0)
		{
			*(ptr + i) = *(s1 + i);
			i++;
		}
		*(ptr + i) = 0;
		return (ptr);
	}
	return (0);
}

int	std_space_get_cut_position(char	*s1)
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
			while (s1 && s1[l] && s1[l] != ' ')
				l++;
	}
	else
		while (s1 && s1[l] && s1[l] != ' ')
			l++;
	return (l);
}
