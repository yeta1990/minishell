/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/25 16:08:04 by albgarci         ###   ########.fr       */
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
//	printf("->%s\n", str);
	if (!*s)
		return (0);
	while (s && *s)
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

static void	save_words(char **ptr, char *str, int nwords, t_data *data)
{
	int		i;
	int		j;
	char	*aux;
	int		len;

	i = 0;
	j = 0;
	len = 0;
	if ((nwords > 0) && str && *str)
	{
		aux = str;
		while (aux && *aux && data->syntax_error == 0)
		{
			j = 0;
			if (has_closed_quotes(aux + len) == 1)
				len = get_char_pos_final_quotes('\'', aux + len) - 1;
			else if (has_closed_quotes(aux + len) == 2)
				len += get_char_pos_final_quotes('\"', aux + len) - 1;
			if (has_closed_quotes(aux + len) == 0) 
			{
				while (aux && aux[len] && aux[len] != '|')
					len++;
			//	printf("aux: %s, len %i\n", aux, len);
				ptr[i++] = ft_substr(aux, 0, len);
			 //	printf("ptr: %s\n", ptr[i - 1]);
				if (parse_check(ptr[i - 1]) == 0)
					syntax_error((aux + len), data);
				if (aux && aux[len] && aux[len] == '|')
					len++;
				if (aux && aux[len] && aux[len] == '|')
					syntax_error((aux + len), data);
				while (aux && aux[j + len] && aux[j + len] == ' ')
					j++;
			//	printf("aux: %s, len %i, j:%i\n", aux + len, len, j);
				if (data->syntax_error == 1)
					break ;
				if ((int)ft_strlen(aux + len) == j && j > 0)
					ptr[i++] = get_cmd_from_user(data);
					//printf("pedir comando\n");
				else if (j == 0 && len > 1 && *(aux + len - 1) == '|')
					ptr[i++] = get_cmd_from_user(data);
					//printf("pedir comando\n");
				else if (j == 0 && len == 1 && aux && *aux == '|')
					syntax_error((aux), data);
				aux +=(len + j);
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

char	**ft_split_pipes(char const *s, t_data *data)
{
	int		nwords;
	char	**ptr;
	char	*str;

	if (s)
	{
		str = ft_strdup(s);
		if (str)
		{
			nwords = get_nwords(str, '|');
			ptr = ft_calloc((nwords + 1), sizeof(char *));
			if (ptr && nwords == 1)
			{
				ptr[0] = ft_strdup(s);
				ptr[1] = 0;
			}
			else if (ptr)
				save_words(ptr, str, nwords, data);
			free(str);
			return (ptr);
		}
	}
	return (0);
}
