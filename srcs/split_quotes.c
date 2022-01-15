/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/15 02:44:18 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nwords(char const *str, char c)
{
	int	nwords;
	int	f_dquote;
	int	f_quote;
	char	*s;

	s = (char *)str;
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
		if (*s != c)
			s++;
		else if ((*s == c) && ((f_dquote && ft_strchr(s, '\"')) || (f_quote && ft_strchr(s, '\''))))
			s += ft_strchr(s, '\"') - s;
		else
		{
			nwords++;
			while (*s == c)
				s++;
		}
	}
	nwords++;
//	printf("words: %i\n", nwords);
	return (nwords);
}

static void	save_words(char **ptr, char *str, char c, int nwords)
{
	int		i;
	char	*aux;
	int		len;

	i = 0;
	len = 0;
	c += 0;

	if ((nwords > 0) && *str)
	{
		aux = str;
		while (*aux)
		{
			if (has_closed_quotes(aux + len) == 1)
				len = get_char_pos_final_quotes('\'', aux + len) - 1;
			else if (has_closed_quotes(aux + len) == 2)
				len += get_char_pos_final_quotes('\"', aux + len) - 1;
			if (has_closed_quotes(aux + len) == 0) 
			{
			//	printf("aux: %s, len %i\n", aux + len, len);
				while (aux && aux[len] && aux[len] != '|')
					len++;
			//	printf("aux: %s, len %i\n", aux, len);
				ptr[i++] = ft_substr(aux, 0, len);
			//	printf("ptr: %s\n", ptr[i - 1]);
				if (aux && aux[len] && aux[len] == '|')
					len++;
				while (aux && aux[len] && aux[len] == ' ')
					aux++;
				aux +=len;
				len = 0;
			}
		}
	ptr[nwords] = 0;
	}
}

int	get_char_pos_final_quotes(char q, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str && str[i])
	{
		while (str[i] != q)
			i++;
		i += 1;
		while (str[i] != q)
			i++;
		if (str[i] && str[i] == q)
			i++;
		return (i + 1);
	}
	return (i);
}

char	**ft_split_w_quotes(char const *s, char c)
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
			if (ptr && nwords == 1)
			{
				ptr[0] = ft_strdup(s);
				ptr[1] = 0;
			}
			else if (ptr)
				save_words(ptr, str, c, nwords);
			free(str);
			return (ptr);
		}
	}
	return (0);
}
