/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:12:15 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 13:59:30 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{	
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*our_getenv(t_data *data, char *to_find)
{
	int		index_env;
	char	*result;
	char	*word_to_find;

	result = 0;
	if (to_find)
	{
		word_to_find = ft_strjoin(to_find, "=");
		index_env = search_word(data->env, word_to_find);
		if (index_env > -1)
			result = ft_strtrim_full_str((data->env)[index_env], word_to_find);
		else
			result = ft_strdup("");
		free(word_to_find);
	}
	return (result);
}
