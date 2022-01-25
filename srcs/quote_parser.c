
//gccw quote_parser.c utils.c utils_2.c ft_strjoin.c ft_memmove.c utils_3.c -I ../inc list_utils*.c free_utils.c ft_is*.c && ./a.out

#include "minishell.h"
int	ft_ft_isalnum(int c);
int	ft_isalpha(int c);
int	ft_isdigit(int c);

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
//	printf("%i\n", i);
	if (i != 0)
		exp = ft_substr(a, 0, i);
	i++;
	if (i == (int) ft_strlen(*arg) && type == 2)
		exp = ft_strdup(*arg);
//	else if (i == 1 && ft_strlen(*arg) == 1 && **arg == '$')
//		exp = ft_strdup(*arg);
	while (a && a[i])
	{
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
			while (a[i + j] && a[i + j] != '$')
			//while (a[i + j] && ft_isalnum(a[i + j]) != 0 && a[i + j] != '$')
			//while (a[i + j] && a[i + j] != '$')
				j++;
			aux_exp = ft_substr(a, i, j);
			aux_exp3 = ft_strdup(getenv(aux_exp));
	//		printf("aux_exp: %s\n", aux_exp);
	//		printf("aux_exp3: %s\n", aux_exp3);
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
			//	printf("tail: %s\n", tail);
				free(tail);
				tail = 0;
			}
			free(aux_exp2);
		}
		i += j;
		i += z;
		j = 0;
		z = 0;
	//	printf("expanded-> %s\n", exp);
	}
	return (exp);
}
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
	int flag;

	flag = 0;
	if (c == '\'')
		flag = 1;
	else if (c == '\"')
		flag = 2;
	return (flag);
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
	//	write(2, "aux\n", 4);
	while (aux && *aux)
	{
	//	write(2, "aux\n", 4);
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
	//		write(2, "au.", 3);
	//		printf("aux: %s\n", aux);
			forward = get_next_separator(aux + 1, what_quotes(flag));
	//		printf("%i\n", forward);
			subs = ft_substr(aux, 0, forward + 1);
	//		printf("subs %s\n", subs);
			trimmed = ft_strtrim(subs, what_quotes_str(flag));
	//		printf("trimmed %s\n", trimmed);
			if (flag == 1)
				expanded = ft_strdup(trimmed);
			else if (flag == 2)
				expanded = expansor(&trimmed, 2, data);
			ft_memcpy(result, expanded, ft_strlen(expanded));
			result += ft_strlen(expanded);
			free(expanded);
			free(subs);
			free(trimmed);
			
	//		write(2, "au2", 3);
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
