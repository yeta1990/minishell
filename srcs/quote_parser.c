/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:09:36 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 12:34:21 by albgarci         ###   ########.fr       */
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

t_split_cmds_vars	*initialise_split_cmds_vars(char *str)
{
	t_split_cmds_vars	*v;

	v = malloc(sizeof(t_split_cmds_vars));
	v->subs = 0;
	v->trimmed = 0;
	v->expanded = 0;
	v->r = ft_calloc((ft_strlen(str) * 50) + 1, sizeof(char));
	v->full_strings = malloc(sizeof(t_files *));
	v->full_strings[0] = 0;
	v->aux = str;
	v->result = v->r;
	v->forward = 0;
	return (v);
}

char	**split_quote_sensitive(char *str, t_data *data)
{
	t_split_cmds_vars	*v;

	v = initialise_split_cmds_vars(str);
	while (v->aux && *v->aux)
	{
		v->flag = what_flag(*v->aux);
		if (v->flag == 0)
		{
			while (v->aux && *v->aux && *v->aux == ' ')
				v->aux++;
			v->forward = get_first_coming_separator(v->aux);
			v->subs = ft_substr(v->aux, 0, v->forward - 1);
			v->expanded = expansor(&v->subs, 0, data);
			ft_memcpy(v->result, v->expanded, ft_strlen(v->expanded));
			v->aux += v->forward - 1;
			v->result += ft_strlen(v->expanded);
			free(v->subs);
			free(v->expanded);
			if (v->aux && ((*v->aux && *v->aux == ' ') || ft_strlen(v->aux) == 0))
			{
				v->result = v->r;
				ft_lstadd_back_files(v->full_strings, ft_lstnew(ft_strdup(v->result)));
				ft_bzero(v->result, ft_strlen(v->result));
			}
		}
		else if (v->flag != 0)
		{
			v->forward = get_next_separator(v->aux + 1, what_quotes(v->flag));
			v->subs = ft_substr(v->aux, 0, v->forward + 1);
			v->trimmed = ft_strtrim(v->subs, what_quotes_str(v->flag));
			if (v->flag == 1)
				v->expanded = ft_strdup(v->trimmed);
			else if (v->flag == 2)
				v->expanded = expansor(&v->trimmed, 2, data);
			ft_memcpy(v->result, v->expanded, ft_strlen(v->expanded));
			v->result += ft_strlen(v->expanded);
			free(v->expanded);
			free(v->subs);
			free(v->trimmed);
			if (get_next_separator(v->aux + 1, what_quotes(v->flag)) == (int) ft_strlen(v->aux + 1) + 1)
				v->aux += v->forward;
			else
				v->aux += v->forward + 1;
			if (v->aux && ((*v->aux && *v->aux == ' ') || ft_strlen(v->aux) == 0))
			{
				v->result = v->r;
				ft_lstadd_back_files(v->full_strings, ft_lstnew(ft_strdup(v->result)));
				ft_bzero(v->result, ft_strlen(v->result));
			}
		}
		while (v->aux && *v->aux && *v->aux == ' ')
			v->aux++;
	}
	free(v->r);
	free(v);
	return (from_list_to_double_char(v->full_strings));
}
