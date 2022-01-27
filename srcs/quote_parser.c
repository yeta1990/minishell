/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:09:36 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 12:52:51 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	add_expanded_value_to_list(t_split_cmds_vars *v)
{
	free(v->subs);
	free(v->expanded);
	if (v->flag != 0)
		free(v->trimmed);
	if (v->aux && ((*v->aux && *v->aux == ' ') || ft_strlen(v->aux) == 0))
	{
		v->result = v->r;
		ft_lstadd_back_files(v->full_strings, ft_lstnew(ft_strdup(v->result)));
		ft_bzero(v->result, ft_strlen(v->result));
	}
}

void	split_ops_without_quotes(t_split_cmds_vars *v, t_data *data)
{
	while (v->aux && *v->aux && *v->aux == ' ')
		v->aux++;
	v->forward = get_first_coming_separator(v->aux);
	v->subs = ft_substr(v->aux, 0, v->forward - 1);
	v->expanded = expansor(&v->subs, 0, data);
	ft_memcpy(v->result, v->expanded, ft_strlen(v->expanded));
	v->aux += v->forward - 1;
	v->result += ft_strlen(v->expanded);
	add_expanded_value_to_list(v);
}

void	split_ops_with_quotes(t_split_cmds_vars *v, t_data *data)
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
	if (get_next_separator(v->aux + 1,
			what_quotes(v->flag)) == (int) ft_strlen(v->aux + 1) + 1)
		v->aux += v->forward;
	else
		v->aux += v->forward + 1;
	add_expanded_value_to_list(v);
}

char	**split_quote_sensitive(char *str, t_data *data)
{
	t_split_cmds_vars	*v;
	t_files				*full_strings;

	full_strings = 0;
	v = initialise_split_cmds_vars(str);
	while (v->aux && *v->aux)
	{
		v->flag = what_flag(*v->aux);
		if (v->flag == 0)
			split_ops_without_quotes(v, data);
		else if (v->flag != 0)
			split_ops_with_quotes(v, data);
		while (v->aux && *v->aux && *v->aux == ' ')
			v->aux++;
	}
	free(v->r);
	full_strings = v->full_strings;
	free(v);
	return (from_list_to_double_char(full_strings));
}
