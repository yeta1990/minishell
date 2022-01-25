/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arrange.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:44:06 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 19:06:47 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_raw_cmd(char *file, char **cmd_ok)
{
	int		pos;

	pos = ft_strlen(file) - 1;
	if (access(file, X_OK) != -1)
	{
		if (cmd_ok)
			*cmd_ok = ft_strdup(file);
		return (1);
	}
	return (0);
}

static char	**get_cut_paths(t_data *data)
{
	char	*raw_path;
	char	**paths;

	raw_path = our_getenv(data, "PATH");
	paths = ft_split_mod(raw_path, ':');
	free(raw_path);
	return (paths);
}

int	is_cmd(char *file, char **cmd_ok, t_data *data)
{
	int		j;
	char	*cmd_try;
	char	**paths;

	j = 0;
	paths = get_cut_paths(data);
	while (paths[j])
	{
		cmd_try = ft_strjoin(paths[j], file);
		if (access(cmd_try, X_OK) != -1)
		{
			if ((cmd_ok) && ft_strlen(cmd_try) > ft_strlen(paths[j]))
				*cmd_ok = ft_strdup(cmd_try);
			free(cmd_try);
			free_paths(paths);
			return (1);
		}
		free(cmd_try);
		j++;
	}
	free_paths(paths);
	*cmd_ok = 0;
	if (file)
		*cmd_ok = ft_strdup(file);
	return (0);
}

char	**create_args(char *raw_cmd, char **cmd, t_data *data)
{
	char	**args;
	int		i;

	i = 0;
	args = split_quote_sensitive(raw_cmd, data);
	if (!(is_raw_cmd(args[0], cmd)))
		is_cmd(args[0], cmd, data);
	return (args);
}
