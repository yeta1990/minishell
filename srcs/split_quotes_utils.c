/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 18:08:32 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 18:08:45 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_after_pipe(int len, int j, char *aux, t_data *data)
{
	if ((int)ft_strlen(aux + len) == j && j > 0)
		return (1);
	else if (j == 0 && len > 1 && *(aux + len - 1) == '|')
		return (1);
	else if (j == 0 && len == 1 && aux && *aux == '|')
	{
		syntax_error((aux), data);
		return (0);
	}
	return (0);
}

char	*search_next_pipe(int *len, char *aux, t_data *data)
{
	char	*piece;

	piece = 0;
	while (aux && aux[*len] && aux[*len] != '|')
		(*len)++;
	piece = ft_substr(aux, 0, *len);
	if (parse_check(piece) == 0)
		syntax_error((aux + *len), data);
	if (aux && aux[*len] && aux[*len] == '|')
		(*len)++;
	if (aux && aux[*len] && aux[*len] == '|')
		syntax_error((aux + *len), data);
	return (piece);
}
