/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_char_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:55:09 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/03 17:55:07 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
//gccw get_char_pos.c -I../inc list_utils.c ../../libft/ft_substr.c ../../libft/ft_memcpy.c ../../libft/ft_strdup.c -o m && ./m | cat -e


#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

int	get_char_pos(char *str, char c)
{
	size_t	i;

	i = 0;
	if (!str)
		return (-1);
	while (str && str[i] && i < ft_strlen(str))
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (0);

}

int	has_closed_quotes(char *str)
{
	int		simple;
	int		dble;
	char	*aux;
	int		close;

	simple = 0;
	dble = 0;
	aux = 0;
	close = 0;
	simple = get_char_pos(str, '\'');
	dble = get_char_pos(str, '\"');
	if ((simple > 0 && dble > 0 && simple < dble) || (simple > 0 && dble <= 0))
	{
		aux = str + simple;
		close = get_char_pos(aux, '\'');
		if (close <= 0)
			return (has_closed_quotes(str + simple));
		else
			return (1);
	}
	else if ((simple > 0 && dble > 0 && dble < simple) || (dble > 0 && simple <= 0))
	{
		aux = str + dble;
		close = get_char_pos(aux, '\"');
		if (close <= 0)
			return (has_closed_quotes(str + dble));
		else
			return (2);
	}
	return (0);
}

int main(void)
{
	char	*str = " \" hola\"en medio\" que tal\"\"\" muy bien";
//	char	*str = "hola que tal";
	int		quotes_type;
	char	*aux;
	int		num_strings;
	t_files	**full_strings; 
		// linked list to store each piece of cmd, instead of 
		// current split in cmd arrange
		// after the list preparation, we will have to convert linked list
		// into char **
	full_strings = malloc(sizeof(t_files *) * 2);
	full_strings[0] = 0;
	full_strings[1] = 0;
	aux = str;
	quotes_type = has_closed_quotes(aux);
	num_strings = 0;
	while (quotes_type != 0)
	{
		printf("%i, %s\n", quotes_type, aux);
		if (quotes_type == 1)
		{
			//action
			//append all strings separated by spaces to the end of the list
		//	printf("--->%s\n", ft_substr(aux, 0, get_char_pos(aux, '\'')));
			if (*aux != '\'')
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\''))));
			//save string from " to " and add it to the end of the list
			while (*aux != '\'')
				aux++;
			//save string from ' to ' and add it to the end of the list
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\'') + 1)));
			aux++;
			while (*aux != '\'')
				aux++;
			aux++;
		}
		else if (quotes_type == 2)
		{
			//action
			//split all strings separated by spaces and append each one 
			//to the end of the list
			if (*aux != '\"')
				ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\"'))));
			while (*aux != '\"')
				aux++;
			ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, '\"') + 1)));
			aux++;
				//save string from ' to ' and add it to the end of the list
			while (*aux != '\"')
				aux++;
			aux++;
		}
		quotes_type = has_closed_quotes(aux);
		num_strings++;
	}

	
	///////////////////////////////////////////
	//// not saving the last part of the string!
	//// also replicate the same logic in the first
	//// and middle parts of the string
	///////////////////////////////////////////
	while (aux && *aux)
	{
		while (aux && *aux && *aux != ' ')
			aux++;
		ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
		aux++;
	//	while (aux && *aux && *aux != ' ')
	//		aux++;
	//	aux++;
	}
	ft_lstadd_back_files(full_strings, ft_lstnew(ft_substr(aux, 0, get_char_pos(aux + 1, ' '))));
	//append all strings separated by spaces to the end of the list
	printf("Num full strings: %i\n", num_strings);

	t_files *f;
	f = *full_strings;
	printf("list\n");
	while (f)
	{
		printf("%s\n", f->name);
		f = f->next;
	}
}
