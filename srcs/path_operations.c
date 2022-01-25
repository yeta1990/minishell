/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:12:15 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 15:31:15 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
char	**get_paths(char *path)
{
	char	**raw_path;

	raw_path = malloc(sizeof(char *) * 2);
	if (path)
	{
		raw_path[0] = ft_strdup(path);
		raw_path[1] = 0;
		return (path_surgery((raw_path), 0));
	}
	return (path_surgery((raw_path), 1));
}

char	**path_surgery(char **path_to_cut, int path_emergency)
{
	char	*e1;
	char	*e2;
	char	**env;

	if (path_to_cut && *path_to_cut && path_emergency == 0)
	{
		e2 = ft_strtrim_full_str(*path_to_cut, "PATH=");
		e1 = ft_strjoin(e2, ":/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
		free_paths(path_to_cut);
		env = ft_split_mod(e1, ':');
		free(e2);
		free(e1);
	}
	else
	{
		e1 = ft_strdup(":/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
		env = ft_split_mod(e1, ':');
		free(e1);
	}
	return (env);
}
*/
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
	word_to_find = ft_strjoin(to_find, "=");
	index_env = search_word(data->env, word_to_find);
	if (index_env > - 1)
		result = ft_strtrim_full_str((data->env)[index_env], word_to_find);
	else
		result = ft_strdup("");
	free(word_to_find);
	return (result);
}
