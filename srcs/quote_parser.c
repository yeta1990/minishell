
#include "minishell.h"

int main()
{
	//char *str = "echo 'uno '\"dos\"'tres'   ";
	char *str = "echo 'uno 'dos'tres'   ";
	printf("%s\n", str);
	char **s;

	s = split_quote_sensitive(str);
	while (s && *s)
	{
		printf("%s\n", *s);
		s++;
	}
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

	first_half = 0;
	second_half = 0;
//simple = 1, double = 2
	flag = 0;
	size = 0;
	full_strings = malloc(sizeof(t_files *));
	full_strings[0] = 0;
	aux = str;
	start = str;
	while (aux && *aux)
	{
		if (*aux == '\'')
			flag = 1;
		else if (*aux == '\"')
			flag = 2;
		if (flag == 0)
		{
			while (aux && *aux && *aux != ' ')
			{
				size++;
				aux++;
			}
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(start, 0, size)));
			while (aux && *aux && *aux == ' ')
				aux++;
			size = 0;
			printf("aux %s\n", aux);
			start = aux;
			flag = 0;
		}
		else if (flag == 1)
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
		}


	//	aux++;
	}
	return (from_list_to_double_char(full_strings));
}
