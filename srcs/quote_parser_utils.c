/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:11:19 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 12:14:29 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_separator(char *str, char sep)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == sep)
			return (i + 1);
		i++;
	}
	if (i > 0)
		return (i + 1);
	return (i);
}

int	get_first_coming_separator(char *str)
{
	int	smple;
	int	dble;
	int	min;

	min = get_next_separator(str, ' ');
	if (min == 0)
		return (0);
	smple = get_next_separator(str, '\'');
	dble = get_next_separator(str, '\"');
	if (smple < min)
		min = smple;
	if (dble < min)
		min = dble;
	return (min);
}

char	what_quotes(int type)
{
	if (type == 1)
		return ('\'');
	else if (type == 2)
		return ('\"');
	return (' ');
}

char	*what_quotes_str(int type)
{
	if (type == 1)
		return ("\'");
	else if (type == 2)
		return ("\"");
	return (" ");
}

int	what_flag(char c)
{
	int	flag;

	flag = 0;
	if (c == '\'')
		flag = 1;
	else if (c == '\"')
		flag = 2;
	return (flag);
}
