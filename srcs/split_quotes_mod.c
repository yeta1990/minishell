/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:03:04 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/26 13:24:54 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*gccw split_quotes_mod.c split_quotes_utils.c utils*.c error_handlers.c get_cmd_from_user.c ft_itoa.c ft_putstr_fd.c ft_strjoin.c ft_memmove.c get_char_pos.c list_utils.c list_utils_2.c free_utils.c -I ../inc -I/Users/albgarci/.brew/opt/readline/include -lreadline
*/
#include "minishell.h"

static void	set_quotes_flags(char c, int *f_dquote, int *f_quote)
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

static int	get_nwords(char const *str, char c)
{
	t_words_number	w;
	char			*s;

	s = (char *)str;
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
	printf("nwords: %i\n", w.nwords);
	return (w.nwords);
}

static void	add_pipe_to_list(t_files **separated_pipes, char *s, int size)
{
	char			*trimmed;
	char			*raw_pipe;

	raw_pipe = 0;
	trimmed = 0;
	raw_pipe = ft_substr(s, 0, size);
	trimmed = ft_strtrim(raw_pipe, "| ");
	ft_lstadd_back_files(separated_pipes, ft_lstnew(ft_strdup(trimmed)));
	free(raw_pipe);
	free(trimmed);
}

static char **save_words(char **ptr, char *str, int nwords, t_data *data, char c)
{
	t_words_number	w;
	char			*s;
	int				i;
	t_files			**separated_pipes;
//	char			*trimmed;
//	char			*raw_pipe;

	i = 0;
//	raw_pipe = 0;
//	trimmed = 0;
	s = (char *)str;
	w.f_dquote = 0;
	w.f_quote = 0;
	w.nwords = 1;
	separated_pipes = malloc(sizeof(t_files *));
	separated_pipes[0] = 0;
	while (s && s[i])
	{
		set_quotes_flags(s[i], &w.f_dquote, &w.f_quote);
		if (s[i] != c)
			i++;
		else if ((s[i] == c) && w.f_dquote && ft_strchr(s + i, '\"'))
			i += ft_strchr(s + i, '\"') - (s + i);
		else if ((s[i] == c) && w.f_quote && ft_strchr(s + i, '\''))
			i += ft_strchr(s + i, '\'') - (s + i);
		else if ((s[i] == c) && w.f_quote && !(ft_strchr(s + i, '\'')))
			w.f_quote = 0;
		else if ((s[i] == c) && w.f_dquote && !(ft_strchr(s + i, '\"')))
			w.f_dquote = 0;
		else
		{
			w.nwords++;
			while (s[i] == c)
				i++;
			add_pipe_to_list(separated_pipes, s, i);
	/*		raw_pipe = ft_substr(s, 0, i);
			trimmed = ft_strtrim(raw_pipe, "| ");
			ft_lstadd_back_files(separated_pipes, ft_lstnew(ft_strdup(trimmed)));
			free(raw_pipe);
			free(trimmed);
			raw_pipe = 0;
			trimmed = 0;*/
//			printf("piece to cut: %s\n", ft_strtrim(ft_substr(s, 0, i), "| "));
			s += i;
			i = 0;
		}
	}
	if (s && ft_strlen(s) > 0)
	{
		add_pipe_to_list(separated_pipes, s, ft_strlen(s));
/*
		raw_pipe = ft_substr(s, 0, ft_strlen(s));
		trimmed = ft_strtrim(raw_pipe, "| ");
		ft_lstadd_back_files(separated_pipes, ft_lstnew(ft_strdup(trimmed)));
		free(raw_pipe);
		free(trimmed);*/
	//	printf("piece to cut: %s\n", ft_strtrim(ft_substr(s, 0, ft_strlen(s)), "| "));
	}
	ptr += 0;
	nwords += 0;
	data->last_code = 0;
	return (from_list_to_double_char(separated_pipes));
}


/*static void	save_words(char **ptr, char *aux, int nwords, t_data *data)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	while (aux && *aux && data->syntax_error == 0)
	{
		j = 0;
		len = get_char_pos_final_quotes(aux + len);
	//	printf("len: %d\n", len);
		if (has_closed_quotes(aux + len) == 0)
		{
			ptr[i++] = search_next_pipe(&len, aux, data);
			while (aux && aux[j + len] && aux[j + len] == ' ')
				j++;
			if (data->syntax_error == 1)
				break ;
			if (search_after_pipe(len, j, aux, data) == 1)
				ptr[i++] = get_cmd_from_user(data);

			aux += (len + j);
		}
	}
	ptr[nwords] = 0;
}*/

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
			nwords = get_nwords(str, '|');
			if (nwords > 0 && str && *str)
				ptr = save_words(ptr, aux, nwords, data, '|');
			free(str);
			return (ptr);
		}
	}
	return (0);
}

int	main(void)
{
	t_data	*data;
	char	**result;
	int		i;
//	char	*str = "echo hola \"|\"";
	char	*str = " echo|ls";
//	char	*str = "\"|\" echo  | \"\" echo adios | echo tres";
//	char	*str = "\"|\" echo  | \"\" echo adios | echo tres";
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
//	system("leaks a.out");
}
