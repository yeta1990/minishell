/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_char_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:55:09 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/09 13:27:29 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_char_pos(char *str, char c)
{
	size_t	i;

	i = 0;
	if (!str)
		return (-1);
	while (str && str[i] && i < ft_strlen(str))
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (ft_strlen(str) + 1);
}

int	has_closed_quotes(char *str)
{
	int		simple;
	int		dble;
	char	*aux;
	int		close;

	simple = 0;
	dble = 0;
	aux = 0;
	close = 0;
	simple = get_char_pos(str, '\'');
	dble = get_char_pos(str, '\"');
	if ((simple > 0 && dble > 0 && simple < dble) || (simple > 0 && dble <= 0))
	{
		aux = str + simple;
		close = get_char_pos(aux, '\'');
		if (close == (int) ft_strlen(aux) + 1)
			return (has_closed_quotes(str + simple));
		else
			return (1);
	}
	else if ((simple > 0 && dble > 0 && dble < simple) || (dble > 0 && simple <= 0))
	{
		aux = str + dble;
		close = get_char_pos(aux, '\"');
		if (close == (int) ft_strlen(aux) + 1)
			return (has_closed_quotes(str + dble));
		else
			return (2);
	}
	return (0);
}

void	cut_outside_quotes(char *aux, t_files **full_strings, char c)
{
	int	cut_len;

	cut_len = 0;
	while (aux && *aux && *aux != c)
	{
		while (aux && *aux && *aux == ' ')
			aux++;
		if (get_char_pos(aux + 1, ' ') < get_char_pos(aux + 1, c) && *aux != c)
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
		else if (*aux != c)
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, c))));
		while (aux && *aux && *aux != ' ' && *aux != c)
			aux++;
	}
}

void	cut_end_quotes(char *aux, t_files **full_strings)
{
	int	cut_len;

	cut_len = 0;
	while (aux && *aux)
	{
		while (aux && *aux && *aux == ' ')
			aux++;
		ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
		while (aux && *aux && *aux != ' ')
			aux++;
	}
}

char **from_list_to_double_char(t_files **full_strings)
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
	s = malloc(sizeof(char *) * size + 1);
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

char **split_quote_sensitive(char *str)
{
	int		quotes_type;
	char	*aux;
	t_files	**full_strings; 

	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	quotes_type = has_closed_quotes(aux);
	while (quotes_type != 0)
	{
		if (quotes_type == 1)
		{
			cut_outside_quotes(aux, full_strings, '\'');
			while (*aux != '\'')
				aux++;
			if (has_closed_quotes(aux) == 1)
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\'') + 1)));
			aux++;
			while (*aux != '\'')
				aux++;
			aux++;
		}
		else if (quotes_type == 2)
		{
			cut_outside_quotes(aux, full_strings, '\"');
			while (*aux != '\"')
				aux++;
			if (has_closed_quotes(aux) == 2)
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\"') + 1)));
			aux++;
			while (*aux != '\"')
				aux++;
			aux++;
		}
		quotes_type = has_closed_quotes(aux);
	}
	cut_end_quotes(aux, full_strings);
	return (from_list_to_double_char(full_strings));
}
