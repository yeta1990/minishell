
//gccw quote_parser.c utils.c utils_2.c ft_strjoin.c ft_memmove.c utils_3.c -I ../inc list_utils*.c free_utils.c && ./a.out

#include "minishell.h"


char	*expansor2(char **arg)
{
	char	*a;
	char	*exp;
	char	*aux_exp;
	char	*aux_exp2;
	char	*aux_exp3;
	int		i;
	int		j;

	j = 0;
	i = 0;
	exp = 0;
	aux_exp = 0;
	aux_exp2 = 0;
	aux_exp3 = 0;
	a = *arg;
	printf("to expand-> %s\n", a);
	while (a && a[i] && a[i] != '$')
		i++;
	if (i != 0)
		exp = ft_substr(a, 0, i);
	i++;
	while (a && a[i])
	{
		while (a[i] == ' ')
			i++;
		while (a[i] == '$')
			i++;
		if (a[i] && a[i] == '?')
		{
		//	aux_exp3 = ft_itoa(data->last_code);
			j++;
		}
		else
		{
			while (a[i + j] && isalnum(a[i + j]) != 0 && a[i + j] != '$')
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
			exp = ft_strdup(aux_exp2);
			free(aux_exp2);
		}
		i += j;
		j = 0;
	//	printf("expanded-> %s\n", exp);
	}
	return (exp);
}

int main()
{
//	char *str = "echo $PATH";
	char *str = "echo\"uno $USER \" a a\"$USER $USER\"holi$USER 'tres'   ";
//	char *str = "echo'''' uno 'dos'tres '' ";
	int	i;

	i = 0;
//	char *str = " echo '' hola ";// uno doos ";
//	char	*str = "echo'ho la'";
	printf("%s\n", str);
	char **s;

	s = split_quote_sensitive(str);
	while (s && s[i])
	{
		printf("->%s\n", s[i]);
		free(s[i]);
		i++;
	}
	free(s);
//	free(str);

//	system("leaks a.out");
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

char	**split_quote_sensitive(char *str)
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
	r = ft_calloc((ft_strlen(str)  * 10)+ 1, sizeof(char));
	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	result = r;
	forward = 0;
	while (aux && *aux)
	{
		if (*aux == '\'')
			flag = 1;
		else if (*aux == '\"')
			flag = 2;
		else
			flag = 0;
		if (flag == 0)
		{
			while (aux && *aux && *aux == ' ')
				aux++;
			forward = get_next_separator(aux, ' ');

			if (get_next_separator(aux, '\'') < forward) 
			{
				forward = get_next_separator(aux, '\'');
				subs = ft_substr(aux, 0, forward - 1);
				expanded = expansor2(&subs);
				ft_memcpy(result, expanded, ft_strlen(expanded));
				free(subs);
				free(expanded);
			//		ft_memcpy(result, aux, forward - 1);
				result += forward - 1;
				aux += forward - 1;
				continue ;
			}
			else if (get_next_separator(aux, '\"') < forward) 
			{
				forward = get_next_separator(aux, '\"');
				subs = ft_substr(aux, 0, forward - 1);
				expanded = expansor2(&subs);
				ft_memcpy(result, expanded, ft_strlen(expanded));
				free(subs);
				free(expanded);
		//		ft_memcpy(result, aux, forward - 1);
				result += forward - 1;
				aux += forward - 1;
				continue ;
			}
			subs = ft_substr(aux, 0, forward - 1);
			expanded = expansor2(&subs);
			ft_memcpy(result, expanded, ft_strlen(expanded));
			free(subs);
			free(expanded);
		//	ft_memcpy(result, aux, forward - 1);
			aux += forward - 1;
 			if (aux && ((*aux && *aux == ' ') || ft_strlen(aux) == 0))
			{
				result = r;
				printf("result: %s\n", result);
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
			}
			else if (aux && ft_strlen(aux) > 0)
			{
				printf("result: %s\n", result);
				result += forward;
				forward = get_next_separator(aux, ' ');
				printf("forward %d\n", forward);
				ft_memcpy(result, aux, forward - 1);
				aux += forward;
				result = r;
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
			}
		//	if (ft_strlen(aux) > 0)
		//		aux++;
		}
		else if (flag != 0)
		{
			if (flag == 1)
			{
				forward = get_next_separator(aux + 1, '\'');
				subs = ft_substr(aux, 0, forward + 1);
				trimmed = ft_strtrim(subs, "\'");
				ft_memcpy(result, trimmed, ft_strlen(trimmed));
				free(subs);
				free(trimmed);
			}
			else if (flag == 2)
			{
				forward = get_next_separator(aux + 1, '\"');
				subs = ft_substr(aux, 0, forward + 1);
				trimmed = ft_strtrim(subs, "\"");
				expanded = expansor2(&trimmed);
				ft_memcpy(result, expanded, ft_strlen(expanded));
				free(subs);
				free(trimmed);
				free(expanded);
			}
			aux += forward + 1;
 			if (aux && ((*aux && *aux == ' ') || ft_strlen(aux) == 0))
			{
				result = r;
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
			}
			else if (aux && ft_strlen(aux) > 0)
			{
				result += forward + 1;
				forward = get_next_separator(aux, ' ');
				if (flag == 1)
				{
					subs = ft_substr(aux, 0, forward - 1);
					trimmed = ft_strtrim(subs, "\'");
					ft_memcpy(result, trimmed, ft_strlen(trimmed));
					free(subs);
					free(trimmed);
					free(expanded);
				}
				else if (flag == 2)
				{
					subs = ft_substr(aux, 0, forward - 1);
					trimmed = ft_strtrim(subs, "\"");
					expanded = expansor2(&trimmed);
					ft_memcpy(result, expanded, ft_strlen(expanded));
					free(subs);
					free(trimmed);
					free(expanded);
				}

				aux += forward;
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
