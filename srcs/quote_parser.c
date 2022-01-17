
//gccw quote_parser.c utils.c utils_2.c ft_strjoin.c ft_memmove.c utils_3.c -I ../inc list_utils*.c free_utils.c && ./a.out

#include "minishell.h"


int main()
{
	//char *str = "echo 'uno '\"dos\"'tres'   ";
//	char *str = "echo'''' uno 'dos'tres ''   ";
	int	i;

	i = 0;
	char *str = " echo hola";// uno doos ";
	printf("%s\n", str);
	char **s;


	s = split_quote_sensitive(str);
	while (s && s[i])
	{
		printf("->%s\n", s[i]);
		free(s[i]);
		i++;
	}
//	free(s);
//	free(str);

//	system("leaks a.out");
}


char	**split_quote_sensitive(char *str)
{
//	int	quotes_type;
	char	*aux;
	char	*start;
	t_files	**full_strings;
	int		flag;
	int		size;
	char	*first_half;
	char	*second_half;
	char	*third_half;
	char	*result;
	char	*r;

	r = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	first_half = 0;
	second_half = 0;
	third_half = 0;
//simple = 1, double = 2
	size = 0;
	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	start = str;
	result = r;
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
			{
				aux++;
				start++;
			}
			start = result;
			while (aux && *aux && *aux != ' ')
			{
				printf("f");

				printf("aux %s\n", aux);
				ft_memcpy(result, aux, 1);
				result++;
				size++;
				aux++;
			}
			result = start;
			printf("%s\n", result);
			if (aux && *aux && *aux == ' ')
			{
				printf(".%s\n", result);
				ft_memcpy(result + size, aux, ft_strlen(aux));
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
				ft_bzero(result, ft_strlen(result));
				//	free(result);
			//	result = 0;
				while (aux && *aux && *aux == ' ')
					aux++;
			}
			size = 0;
			if (ft_strlen(aux) == 0)
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strdup(result)));
			printf("%s\n", result);
			printf("aux 1%s\n", aux);
			start = aux;
		}
	/*	else if (flag == 1)
		{
			printf("result->%s\n", result);
			size++;
			aux++;
			while (aux && *aux && *aux != '\'')
			{
				size++;
				aux++;
			}
			if (aux && *aux && *aux == '\'')
			{
				size++;
				aux++;
			}
			if (aux && *aux && *aux == ' ')
			{
			//	free(first_half);
			//	free(second_half);
				first_half = result;
				second_half = ft_substr(start, 0, size);
				if (result)
				{
			//		free(result);
					result = 0;
				}
				if (first_half && second_half)
					result = ft_strjoin(first_half, second_half);
				else if (first_half)
				{
				//	result = ft_strdup(first_half);
				//	free(first_half);
				//	first_half = 0;
				//	printf("result 9: %s\n", result);
				}
				else if (second_half)
				{
					result = ft_strdup(second_half);
					free(second_half);
					second_half = 0;
					printf("result 99: %s\n", result);
				}
				printf("result 999: %s\n", result);
				ft_lstadd_back_files(full_strings, ft_lstnew(result));
				free(result);
				result = 0;
				while (aux && *aux && *aux == ' ')
					aux++;
				printf("aux 9 %s\n", aux);
			}
			else
			{
				printf("result->%s\n", result);
				printf("aux 88%s\n", aux);

				printf("first: %s, second: %s\n", first_half, second_half);
				free(first_half);
				first_half = result;
				second_half = ft_substr(start, 0, size);

				printf("first: %s, second: %s\n", first_half, second_half);

				printf("result->%s\n", result);
			//	first_half = ft_strjoin(ft_strdup(result), second_half);

				printf("first: %s, second: %s\n", first_half, second_half);
			//	free(second_half);
			//	second_half = 0;
				if (result)
				{
					free(result);
					result = 0;
				}
				if (first_half && second_half)
				{
					printf("1.first: %s, second: %s\n", first_half, second_half);
					result = ft_strjoin(first_half, second_half);
					free(first_half);
					free(second_half);
				}
				else if (first_half)
				{
				//	result = ft_strdup(first_half);
				//	free(first_half);
				//	first_half = 0;
					printf("result 8: %s\n", result);
				}
				else if (second_half)
				{
					result = ft_strdup(second_half);
					free(second_half);
					second_half = 0;
					printf("result 88: %s\n", result);
				}
				printf("result 888: %s\n", result);
			//	free(first_half);
				start = aux;
			}
			size = 0;
			if (ft_strlen(aux) == 0)
				ft_lstadd_back_files(full_strings, ft_lstnew(result));
			printf("aux 88%s\n", aux);
			start = aux;

		}*/
	/*	else if (flag == 1)
		{
			start = aux;
			printf("aux1 %s\n", aux);
			size++;
			aux++;
			while (aux && *aux && *aux != '\'')
			{
				size++;
				aux++;
			}
			if (aux && *aux)
			{
				size++;
				aux++;
			}
			if ((aux && *aux && *aux == ' ') || !aux)
			{
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_strtrim(ft_substr(start, 0, size), "\'")));
				while (aux && *aux && *aux == ' ')
					aux++;
				size = 0;
				start = aux;

			}
			else
			{
				first_half = ft_strtrim(ft_substr(start, 0, size), "\'");
				second_half = ft_substr(start, size, ft_strlen(start) - size);
			//	free(start);
				start = ft_strjoin(first_half, second_half);
				free(first_half);
				free(second_half);
			//	ft_lstadd_back_files(full_strings, ft_lstnew(ft_strtrim(ft_substr(start, 0, size), "\'")));
				
			
			}
			flag = 0;
		}
		else if (flag == 2)
		{
			start = aux;
			printf("aux2 %s\n", aux);
			size++;
			aux++;
			while (aux && *aux && *aux != '\"')
			{
				size++;
				aux++;
			}
			if (aux && *aux)
			{
				size++;
				aux++;
			}
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(start, 0, size)));
			while (aux && *aux && *aux == ' ')
				aux++;
			size = 0;
			start = aux;
			flag = 0;
		}*/


	//	aux++;
	}
	return (from_list_to_double_char(full_strings));
}
