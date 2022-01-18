
//gccw quote_parser.c utils.c utils_2.c ft_strjoin.c ft_memmove.c utils_3.c -I ../inc list_utils*.c free_utils.c && ./a.out

#include "minishell.h"


int main()
{
	char *str = "echo 'uno '\"'dos'\"'tres'   ";
//	char *str = "echo'''' uno 'dos'tres '' ";
	int	i;

	i = 0;
//	char *str = " echo hola ";// uno doos ";
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
	return (i);
}

char	**split_quote_sensitive(char *str)
{
	char	*aux;
	char	*start;
	t_files	**full_strings;
	int		flag;
	int		size;
	char	*result;
	char	*r;

	r = ft_calloc(ft_strlen(str) + 1, sizeof(char));
//simple = 1, double = 2
	size = 0;
	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	start = str;
	result = r;
	int	forward;
	forward = 0;
	while (aux && *aux)
	{
		if (*aux == '\'')
			flag = 1;
		else if (*aux == '\"')
			flag = 2;
		else
			flag = 0;

		printf("first aux: %s\n", aux);
		if (flag == 0)
		{
			while (aux && *aux && *aux == ' ')
				aux++;
			forward = get_next_separator(aux, ' ');
			printf("next cut: %i", forward);
			ft_memcpy(result, aux, forward - 1);
			aux += forward - 1;
			printf("\naux-> %s\n", aux);
			if (aux && *aux && *aux == ' ')
			{
				printf(".%s\n", result);
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				result = r;
				ft_bzero(result, ft_strlen(result));
				//	free(result);
			//	result = 0;

			}
			else if (aux && ft_strlen(aux) > 0)
			{
				printf("hhh\n");

				printf(".%s\n", result);
				result += forward;
				forward = get_next_separator(aux, ' ');
				printf("next cut: %i\n", forward);
				ft_memcpy(result, aux, forward - 1);
				aux += forward;
				result = r;

				printf("RES%s\n", result);
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				result = r;
				ft_bzero(result, ft_strlen(result));

			}
			else if (ft_strlen(aux) == 0)
			{
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				result = r;
				ft_bzero(result, ft_strlen(result));
			}
			aux++;
		//	printf("aux 1%s\n", aux);
		}
		else if (flag == 1)
		{
			forward = get_next_separator(aux + 1, '\'');
			printf("next cut: %i\n", forward + 1);
			ft_memcpy(result, aux, forward + 1);
			aux += forward + 1;
			printf("res->%s\n", result);
			printf("len aux: %zu\n", ft_strlen(aux));
 			if (aux && ((*aux && *aux == ' ') || (int)ft_strlen(aux) == 0))
			{
				printf("aux11: %s\n", aux);
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				result = r;
				ft_bzero(result, ft_strlen(result));
			}
			else if (aux && ft_strlen(aux) > 0)
			{
				printf(".%s\n", result);
				result += forward + 1;
				printf(".%s\n", result);
				printf("aux22: %s\n", aux);
				forward = get_next_separator(aux, ' ');
				printf("next cut: %i\n\n", forward);
				ft_memcpy(result, aux, forward - 1);
				printf("!! %s\n", result);
				aux += forward;
				result = r;
				printf("!! %s\n", result);
				printf("aux22: %s\n", aux);
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				result = r;
				ft_bzero(result, ft_strlen(result));
			}
		}
		while (aux && *aux && *aux == ' ')
			aux++;

	}
	free(r);
	return (from_list_to_double_char(full_strings));
}
