/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/26 10:32:06 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_quotes_flags(char c, int *f_dquote, int *f_quote)
{
	if ((c == '"') && *f_dquote == 0 && *f_quote == 0)
		*f_dquote = 1;
	else if ((c == '"') && *f_dquote == 1)
		*f_dquote = 0;
	if ((c == '\'') && *f_quote == 0 && *f_dquote == 0)
		*f_quote = 1;
	else if ((c == '\'') && *f_quote == 1)
		*f_quote = 0;

		printf("hh\n");
}

static int	get_nwords(char const *str, char c)
{
	t_words_number	w;
	char			*s;

	s = (char *)str;
	w.f_dquote = 0;
	w.f_quote = 0;
	w.nwords = 1;
	if (!*s)
		return (0);
	while (s && *s)
	{
		set_quotes_flags(*s, &w.f_dquote, &w.f_quote);
		if (*s != c)
			s++;
		else if ((*s == c) && ((w.f_dquote && ft_strchr(s, '\"'))
				|| (w.f_quote && ft_strchr(s, '\''))))
			s += ft_strchr(s, '\"') - s;
		else
		{
			w.nwords++;
			while (*s == c)
				s++;
		}
	}
	return (w.nwords);
}

static void	save_words(char **ptr, char *aux, int nwords, t_data *data)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	while (aux && *aux && data->syntax_error == 0)
	{
		j = 0;
		len = get_char_pos_final_quotes(aux + len);
		if (has_closed_quotes(aux + len) == 0)
		{
			ptr[i++] = search_next_pipe(&len, aux, data);
			printf("ptr: %s\n", ptr[i - 1]);
			while (aux && aux[j + len] && aux[j + len] == ' ')
				j++;
			if (data->syntax_error == 1)
				break ;
			if (search_after_pipe(len, j, aux, data) == 1)
				ptr[i++] = get_cmd_from_user(data);
			aux += (len + j);
		}
	}
	ptr[nwords] = 0;
}

int	get_char_pos_final_quotes(char *str)
{
	int		i;
	char	q;

	if (has_closed_quotes(str) == 0)
		return (0);
	if (has_closed_quotes(str) == 1)
		q = '\'';
	else if (has_closed_quotes(str) == 2)
		q = '\"';
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
		return (i);
	}
	return (i);
}

char	**ft_split_pipes(char const *s, t_data *data)
{
	int		nwords;
	char	**ptr;
	char	*str;
	char	*aux;

	if (s)
	{
		str = ft_strdup(s);
		aux = str;
		if (str)
		{
			nwords = get_nwords(str, '|');
			ptr = ft_calloc((nwords + 1), sizeof(char *));
			if (ptr && nwords == 1)
			{
				ptr[0] = ft_strdup(s);
				ptr[1] = 0;
			}
			else if (ptr && nwords > 0 && str && *str)
				save_words(ptr, aux, nwords, data);
			free(str);
			return (ptr);
		}
	}
	return (0);
}
