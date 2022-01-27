/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:50:22 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 13:14:31 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_expansor_vars	*initialise_expansor_vars(char **arg)
{
	t_expansor_vars	*v;

	v = malloc(sizeof(t_expansor_vars));
	v->j = 0;
	v->i = 0;
	v->z = 0;
	v->exp = 0;
	v->aux_exp = 0;
	v->aux_exp2 = 0;
	v->aux_exp3 = 0;
	v->tail = 0;
	v->a = *arg;
	return (v);
}

void	expansor_first(t_expansor_vars *v, char **arg, int type)
{
	while (v->a && v->a[v->i] && v->a[v->i] != '$')
		v->i++;
	if (v->i != 0)
		v->exp = ft_substr(v->a, 0, v->i);
	v->i++;
	if (v->i == (int) ft_strlen(*arg) && type == 2)
		v->exp = ft_strdup(*arg);
}

void	expansor_second(t_expansor_vars *v, t_data *data)
{
	if (v->a[v->i] && v->a[v->i] == '?')
	{
		v->aux_exp3 = ft_itoa(data->last_code);
		v->j++;
	}
	else
	{
		while (v->a[v->i + v->j] && ft_isalnum(v->a[v->i + v->j]) != 0
			&& v->a[v->i + v->j] != '$')
			v->j++;
		v->aux_exp = ft_substr(v->a, v->i, v->j);
		v->aux_exp3 = our_getenv(data, v->aux_exp);
		if (v->aux_exp)
			free(v->aux_exp);
	}
	if (v->exp)
		v->aux_exp2 = ft_strjoin(v->exp, v->aux_exp3);
	else if (v->aux_exp3)
		v->aux_exp2 = ft_strdup(v->aux_exp3);
}

void	expansor_third(t_expansor_vars *v)
{
	free(v->aux_exp3);
	if (v->aux_exp2)
	{
		if (v->exp)
			free(v->exp);
		while (v->a && v->a[v->i + v->j + v->z] && v->a[v->i + v->j + v->z]
			!= '$')
			v->z++;
		if (v->z == 0)
			v->exp = ft_strdup(v->aux_exp2);
		else
		{
			v->tail = ft_substr(v->a, v->i + v->j, v->z);
			v->exp = ft_strjoin(v->aux_exp2, v->tail);
			free(v->tail);
			v->tail = 0;
		}
		free(v->aux_exp2);
	}
	v->i += v->j;
	v->i += v->z;
	while (v->a[v->i] && ft_isalnum(v->a[v->i]) == 0 && v->a[v->i] != '$')
		v->i++;
	v->j = 0;
	v->z = 0;
}

char	*expansor(char **arg, int type, t_data *data)
{
	t_expansor_vars	*v;

	v = initialise_expansor_vars(arg);
	if (ft_strchr(v->a, '$') == 0 || type == 1)
		return (ft_strdup(*arg));
	expansor_first(v, arg, type);
	while (v->a && v->a[v->i])
	{
		if (ft_strchr(v->a + v->i - 1, '$') == 0)
			return (ft_strdup(*arg));
		while (v->a[v->i] == ' ')
			v->i++;
		while (v->a[v->i] == '$')
			v->i++;
		if (v->i == (int) ft_strlen(v->a) && type == 2)
			return (ft_strdup(""));
		expansor_second(v, data);
		expansor_third(v);
	}
	free(v);
	return (v->exp);
}
