/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arrange.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:44:06 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/14 18:20:02 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_raw_cmd(char *file, char **cmd_ok)
{
	int		pos;
	char	*aux;

	pos = ft_strlen(file) - 1;
	if (access(file, X_OK) != -1)
	{
		if (cmd_ok)
			*cmd_ok = ft_strdup(file);
		while (pos >= 0)
		{
			if (file[pos] == '/')
			{
				aux = ft_substr(file, pos, ft_strlen(file) - pos + 1);
				free(file);
				file = aux;
			}
			pos--;
		}
		return (1);
	}
	return (0);
}

int	is_cmd(char *file, char **cmd_ok)
{
	int		j;
	char	*cmd_try;
	char	**paths;

	j = 0;
	paths = get_paths(getenv("PATH"));
	while (paths[j])
	{
		cmd_try = ft_strjoin(paths[j], file);
		if (access(cmd_try, X_OK) != -1)
		{
			if (cmd_ok)
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
	char	*aux;
	int		i;

	i = 0;
	args = split_quote_sensitive(raw_cmd);
	if (args)
	{
		while (args && args[i])
		{
			aux = args[i];
			aux = ft_strtrim(args[i], "\"");
			free(args[i]);
			if (ft_strlen(aux) > 1 && aux[0] == '$')
			{
				if (aux[1] == '?')
					args[i] = ft_itoa(data->last_code);
				else
				{
					args[i] = ft_strdup(getenv(++aux));
					aux--;
				}
			}
			else
				args[i] = ft_strdup(aux);
			if (aux)
				free(aux);
			i++;
		}
	}
	if (!(is_raw_cmd(args[0], cmd)))
		is_cmd(args[0], cmd);
	return (args);
}
