/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arrange.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:44:06 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/03 12:45:33 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_is_builtin(char *file, char **cmd_ok)
{
	int	b;

	b = 0;
	if (ft_strcmp("cd", file) == 0)
		b = 1;
	else if (ft_strcmp("pwd", file) == 0)
		b = 1;
	else if (ft_strcmp("env", file) == 0)
		b = 1;
	else if (ft_strcmp("export", file) == 0)
		b = 1;
	else if (ft_strcmp("unset", file) == 0)
		b = 1;
	else if (ft_strcmp("echo", file) == 0)
		b = 1;
	else if (ft_strcmp("exit", file) == 0)
		b = 1;
	if (cmd_ok && b == 1)
		*cmd_ok = ft_strdup(file);
	return (b);
}

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

	j = -1;
	paths = get_cut_paths(data);
	while (paths[++j])
	{
		cmd_try = ft_strjoin(paths[j], file);
		if (access(cmd_try, X_OK) != -1)
		{
			if ((cmd_ok) && ft_strlen(cmd_try) > ft_strlen(paths[j]))
				*cmd_ok = ft_strdup(cmd_try);
			else
				*cmd_ok = ft_strdup("");
			free(cmd_try);
			free_paths(paths);
			return (1);
		}
		free(cmd_try);
	}
	free_paths(paths);
	*cmd_ok = ft_strdup(file);
	return (0);
}

char	**create_args(char *raw_cmd, char **cmd, t_data *data)
{
	char	**args;
	int		i;

	i = 0;
	args = split_quote_sensitive(raw_cmd, data);
	if (!(is_raw_cmd(args[0], cmd)) && !(cmd_is_builtin(args[0], cmd)))
		is_cmd(args[0], cmd, data);
	if (!args)
		return (0);
	return (args);
}
