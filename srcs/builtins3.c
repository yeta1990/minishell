/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:51:43 by crisfern          #+#    #+#             */
/*   Updated: 2022/01/21 17:36:21 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_word_del(char **arr, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j] && str[j])
		{
			if (str[j] != arr[i][j])
				break ;
			else if ((!str[j + 1] && (arr[i][j + 1] == '='))
				|| (!str[j + 1] && !arr[i][j + 1]))
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int	search_word(char **arr, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j] && str[j])
		{
			if (str[j] != arr[i][j])
				break ;
			else if (((str[j + 1] == '=') && (arr[i][j + 1] == '='))
				|| ((str[j + 1] == '=') && !arr[i][j + 1]) || (!str[j + 1]
				&& (arr[i][j + 1] == '=')) || (!str[j + 1] && !arr[i][j + 1]))
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

char	**add_entry(char **old_arr, char *new_str)
{
	int		i;
	int		size;
	char	**arr;

	i = 0;
	size = get_env_size(old_arr) + 1;
	arr = ft_calloc(size + 1, sizeof(char *));
	while (old_arr[i])
	{
		arr[i] = old_arr[i];
		i++;
	}
	arr[i] = new_str;
	free(old_arr);
	return (arr);
}

char	**del_entry(char **old_arr, int index)
{
	int		i;
	int		j;
	int		size;
	char	**arr;

	i = 0;
	j = 0;
	size = get_env_size(old_arr);
	arr = ft_calloc(size, sizeof(char *));
	while (old_arr[i])
	{
		if (i != index)
			arr[j++] = old_arr[i];
		i++;
	}
	free(old_arr);
	return (arr);
}
