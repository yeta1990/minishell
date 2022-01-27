/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:09:36 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 12:13:08 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansor(char **arg, int type, t_data *data)
{
	char	*a;
	char	*exp;
	char	*aux_exp;
	char	*aux_exp2;
	char	*aux_exp3;
	char	*tail;
	int		i;
	int		j;
	int		z;

	j = 0;
	i = 0;
	z = 0;
	exp = 0;
	aux_exp = 0;
	aux_exp2 = 0;
	aux_exp3 = 0;
	tail = 0;
	a = *arg;
	if (ft_strchr(a, '$') == 0 || type == 1)
		return (ft_strdup(*arg));
	while (a && a[i] && a[i] != '$')
		i++;
	if (i != 0)
		exp = ft_substr(a, 0, i);
	i++;
	if (i == (int) ft_strlen(*arg) && type == 2)
		exp = ft_strdup(*arg);
	while (a && a[i])
	{
		if (ft_strchr(a + i - 1, '$') == 0)
			return (ft_strdup(*arg));
		while (a[i] == ' ')
			i++;
		while (a[i] == '$')
			i++;
		if (i == (int) ft_strlen(a) && type == 2)
			return (ft_strdup(""));
		if (a[i] && a[i] == '?')
		{
			aux_exp3 = ft_itoa(data->last_code);
			j++;
		}
		else
		{
			while (a[i + j] && ft_isalnum(a[i + j]) != 0 && a[i + j] != '$')
				j++;
			aux_exp = ft_substr(a, i, j);
			aux_exp3 = our_getenv(data, aux_exp);
			if (aux_exp)
				free(aux_exp);
		}
		if (exp)
			aux_exp2 = ft_strjoin(exp, aux_exp3);
		else if (aux_exp3)
			aux_exp2 = ft_strdup(aux_exp3);
		free(aux_exp3);
		if (aux_exp2)
		{
			if (exp)
				free(exp);
			while (a && a[i + j + z] && a[i + j + z] != '$')
				z++;
			if (z == 0)
				exp = ft_strdup(aux_exp2);
			else
			{
				tail = ft_substr(a, i + j, z);
				exp = ft_strjoin(aux_exp2, tail);
				free(tail);
				tail = 0;
			}
			free(aux_exp2);
		}
		i += j;
		i += z;
		while (a[i] && ft_isalnum(a[i]) == 0 && a[i] != '$')
			i++;
		j = 0;
		z = 0;
	}
	return (exp);
}

char	**split_quote_sensitive(char *str, t_data *data)
{
	char	*aux;
	t_files	**full_strings;
	int		flag;
	char	*result;
	char	*r;
	char	*trimmed;
	char	*subs;
	char	*expanded;
	int	forward;

	subs = 0;
	trimmed = 0;
	expanded = 0;
	r = ft_calloc((ft_strlen(str)  * 50)+ 1, sizeof(char));
	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	result = r;
	forward = 0;
	while (aux && *aux)
	{
		flag = what_flag(*aux);
		if (flag == 0)
		{
			while (aux && *aux && *aux == ' ')
				aux++;
			forward = get_first_coming_separator(aux);
			subs = ft_substr(aux, 0, forward - 1);
			expanded = expansor(&subs, 0, data);
			ft_memcpy(result, expanded, ft_strlen(expanded));
			aux += forward - 1;
			result += ft_strlen(expanded);
			free(subs);
			free(expanded);
 			if (aux && ((*aux && *aux == ' ') || ft_strlen(aux) == 0))
			{
				result = r;
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
			}
		}
		else if (flag != 0)
		{
			forward = get_next_separator(aux + 1, what_quotes(flag));
			subs = ft_substr(aux, 0, forward + 1);
			trimmed = ft_strtrim(subs, what_quotes_str(flag));
			if (flag == 1)
				expanded = ft_strdup(trimmed);
			else if (flag == 2)
				expanded = expansor(&trimmed, 2, data);
			ft_memcpy(result, expanded, ft_strlen(expanded));
			result += ft_strlen(expanded);
			free(expanded);
			free(subs);
			free(trimmed);
			if (get_next_separator(aux + 1, what_quotes(flag)) == (int) ft_strlen(aux + 1) + 1)
				aux += forward;
			else
				aux += forward + 1;
 			if (aux && ((*aux && *aux == ' ') || ft_strlen(aux) == 0))
			{
				result = r;
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
			}


		}
		while (aux && *aux && *aux == ' ')
			aux++;
	}
	free(r);
	return (from_list_to_double_char(full_strings));
}
