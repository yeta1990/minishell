/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 18:08:32 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 21:32:47 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_quotes_flags(char c, int *f_dquote, int *f_quote)
{
	if ((c == '"') && *f_dquote == 0 && *f_quote == 0)
		*f_dquote = 1;
	else if ((c == '"') && *f_dquote == 1)
		*f_dquote = 0;
	if ((c == '\'') && *f_quote == 0 && *f_dquote == 0)
		*f_quote = 1;
	else if ((c == '\'') && *f_quote == 1)
		*f_quote = 0;
}

int	get_nwords(char *s, char c)
{
	t_pipe_sep_vars	w;

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
		else if ((*s == c) && w.f_dquote && ft_strchr(s, '\"'))
			s += ft_strchr(s, '\"') - s;
		else if ((*s == c) && w.f_quote && ft_strchr(s, '\''))
			s += ft_strchr(s, '\'') - s;
		else
		{
			w.nwords++;
			while (*s == c)
				s++;
		}
	}
	return (w.nwords);
}

int	check_pipes_syntax_error(char *str, t_data *data)
{
	int	i;

	i = ft_strlen(str);
	if (!str)
		return (1);
	if (i == 0)
		return (0);
	if (i == 1)
	{
		if (*str == '|')
		{
			syntax_error(str, data);
			return (1);
		}
	}
	i--;
	while (str && str[i] && str[i] == ' ')
		i--;
	if (str[i] == '|' && str[i - 1] == '|')
	{
		syntax_error(str + i, data);
		return (1);
	}
	return (0);
}

void	add_pipe(t_files **sep_pipes, char *s, t_pipe_sep_vars *w, t_data *data)
{
	char			*trimmed;
	char			*raw_pipe;

	raw_pipe = 0;
	trimmed = 0;
	if (check_pipes_syntax_error(s, data) == 1)
		return ;
	raw_pipe = ft_substr(s, 0, w->i);
	trimmed = ft_strtrim(raw_pipe, "| ");
	if (w->last == 0 && ft_strlen(trimmed) == 0)
		syntax_error("|", data);
	else if (w->last == 1 && ft_strlen(trimmed) == 0 && data->syntax_error == 0)
	{
		free(trimmed);
		trimmed = get_cmd_from_user(data);
	}
	ft_lstadd_back_files(sep_pipes, ft_lstnew(ft_strdup(trimmed)));
	free(raw_pipe);
	free(trimmed);
}

t_pipe_sep_vars	*initialise_save_words_vars(void)
{
	t_pipe_sep_vars	*w;
	t_files			**separated_pipes;

	w = malloc(sizeof(t_pipe_sep_vars));
	w->f_dquote = 0;
	w->f_quote = 0;
	w->i = 0;
	w->last = 0;
	w->nwords = 0;
	separated_pipes = malloc(sizeof(t_files *));
	separated_pipes[0] = 0;
	w->separated_pipes = separated_pipes;
	return (w);
}
