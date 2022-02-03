/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/02/03 12:05:25 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_cut_sizes(t_sep *w, char *s, char c)
{
	if (s[w->i] != c)
		w->i++;
	else if ((s[w->i] == c) && w->f_dquote && ft_strchr(s + w->i, '\"'))
		w->i += ft_strchr(s + w->i, '\"') - (s + w->i);
	else if ((s[w->i] == c) && w->f_quote && ft_strchr(s + w->i, '\''))
		w->i += ft_strchr(s + w->i, '\'') - (s + w->i);
	else if ((s[w->i] == c) && w->f_quote && !(ft_strchr(s + w->i, '\'')))
		w->f_quote = 0;
	else if ((s[w->i] == c) && w->f_dquote && !(ft_strchr(s + w->i, '\"')))
		w->f_dquote = 0;
	else
		return (0);
	return (1);
}

static char	**save_words(char *str, t_data *data, char c)
{
	t_sep	*w;
	char	*s;

	s = (char *)str;
	w = initialise_save_words_vars();
	while (s && s[w->i] && data->syntax_error == 0)
	{
		set_quotes_flags(s[w->i], &w->f_dquote, &w->f_quote);
		if (set_cut_sizes(w, s, c) == 0)
		{
			while (s[w->i] == c)
				w->i++;
			add_pipe(w->separated_pipes, s, w, data);
			s += w->i;
			w->i = 0;
		}
	}
	if (s && data->syntax_error == 0)
	{
		w->last = 1;
		w->i = ft_strlen(s);
		add_pipe(w->separated_pipes, s, w, data);
	}
	free(w);
	return (from_list_to_double_char(w->separated_pipes));
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

static char	**copy_word_to_double_ptr(char *str)
{
	char	**ptr;

	ptr = malloc(sizeof(char *) * 2);
	ptr[0] = ft_strdup(str);
	ptr[1] = 0;
	return (ptr);
}

char	**ft_split_pipes(char const *s, t_data *data)
{
	int		nwords;
	char	**ptr;
	char	*str;
	char	*aux;

	ptr = 0;
	if (s)
	{
		str = ft_strdup(s);
		if (str)
		{
			aux = str;
			nwords = get_nwords(aux, '|');
			if (nwords > 1 && str && *str)
				ptr = save_words(str, data, '|');
			else if (nwords == 1)
				ptr = copy_word_to_double_ptr(str);
			free(str);
			return (ptr);
		}
	}
	return (0);
}
