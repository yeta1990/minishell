/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2021/12/01 11:43:27 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nwords(char const *s, char c)
{
	int	nwords;
	int	f_dquote;
	int	f_quote;

	f_dquote = 0;
	f_quote = 0;
	nwords = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		if ((*s == '"') && f_dquote == 0 && f_quote == 0)
			f_dquote = 1;
		else if ((*s == '"') && f_dquote == 1)
			f_dquote = 0;
		if ((*s == '\'') && f_quote == 0 && f_dquote == 0)
			f_quote = 1;
		else if ((*s == '\'') && f_quote == 1)
			f_quote = 0;
		if ((*s != c) || ((*s == c) && ((f_dquote && ft_strchr(s, '"')) || (f_quote && ft_strchr(s, '\'')))))
			s++;
		else
		{
			nwords++;
			while (*s == c)
				s++;
		}
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
			nwords = get_nwords(str, c);
			ptr = ft_calloc((nwords + 1), sizeof(char *));
			if (ptr)
				save_words(ptr, str, c, nwords);
			free(str);
			return (ptr);
		}
	}
	return (0);
}


char	*ft_strdup_space(const char *s1, int *size)
{
	size_t	i;
    size_t  len;
	char	*ptr;

	i = 0;
    len = 0;
    while (s1 && s1[len] && s1[len] != ' ' && s1[len] != '<' && s1[len] != '>')
        len++;
    ptr = malloc(sizeof(char) * (len + 1));
    if (ptr)
	{
		*size += len;
		while (len-- > 0)
		{
			*(ptr + i) = *(s1 + i);
			i++;
		}
		*(ptr + i) = 0;
		return (ptr);
	}
	return (0);
}

int main(void)
{
	printf("%d\n", get_nwords("echo \"|\"", '|'));
}