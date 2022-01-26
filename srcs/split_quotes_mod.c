/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/26 15:11:39 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*gccw split_quotes_mod.c split_quotes_utils.c utils*.c error_handlers.c get_cmd_from_user.c ft_itoa.c ft_putstr_fd.c ft_strjoin.c ft_memmove.c get_char_pos.c list_utils.c list_utils_2.c free_utils.c -I ../inc -I/Users/albgarci/.brew/opt/readline/include -lreadline
*/
#include "minishell.h"

int	set_cut_sizes(t_pipe_sep_vars *w, char *s, char c)
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
	t_pipe_sep_vars	*w;
	char			*s;

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
		aux = str;
		if (str)
		{
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
/*
int	main(void)
{
	t_data	*data;
	char	**result;
	int		i;
//	char	*str = "     | echo ";
//	char	*str = "echo hola";
	char	*str = " echo|ls|";
//	char	*str = "\"|\" echo  | \"\" echo adios | echo tres";
//	char	*str = "\"| echo \' |\' \"\" echo adios | echo tres";
	//char	*str = "echo  |\" \"\" echo adios | echo tres";

	i = 0;
	data = malloc(sizeof(t_data));
	data->cmds = malloc(sizeof(t_cmd *));
	data->cmds[0] = 0;
	data->num_cmds = 0;
	data->syntax_error = 0;
	data->cmd_by_stdin = 0;
//	printf("original %s\n", str);
	result = ft_split_pipes(str, data);
	while (result && result[i])
	{
		printf("->%s\n", result[i]);
		i++;
	}
	free_data(data);
	free_double_string(result);
	system("leaks a.out");
}*/
