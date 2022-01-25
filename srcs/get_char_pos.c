/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_char_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:55:09 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 18:43:39 by albgarci         ###   ########.fr       */
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
	int		smpl;
	int		dble;
	int		close;

	smpl = get_char_pos(str, '\'');
	dble = get_char_pos(str, '\"');
	if (get_char_pos(str, '|') < smpl && get_char_pos(str, '|') < dble)
		return (0);
	if ((smpl > 0 && dble > 0 && smpl < dble) || (smpl > 0 && dble <= 0))
	{
		close = get_char_pos(str + smpl, '\'');
		if (close == (int) ft_strlen(str + smpl) + 1)
			return (has_closed_quotes(str + smpl));
		return (1);
	}
	else if ((smpl > 0 && dble > 0 && dble < smpl) || (dble > 0 && smpl <= 0))
	{
		close = get_char_pos(str + dble, '\"');
		if (close == (int) ft_strlen(str + dble) + 1)
			return (has_closed_quotes(str + dble));
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
			ft_lstadd_back_files(full_strings,
				ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
		else if (*aux != c)
			ft_lstadd_back_files(full_strings,
				ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, c))));
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
		ft_lstadd_back_files(full_strings,
			ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
		while (aux && *aux && *aux != ' ')
			aux++;
	}
}
