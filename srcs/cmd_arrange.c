/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arrange.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:44:06 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/11 13:32:21 by albgarci         ###   ########.fr       */
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
	return (0);
}

char	**create_args(char *raw_cmd, char **cmd)
{
	char	**args;
	
//	printf("raw_cmd: %s\n", raw_cmd);
	args = split_quote_sensitive(raw_cmd);
	//in case of a wrong command, it forced an exit, but
	//I'm not sure whether this is the best place to check it, or 
	//it would be better in the executor
/*	if (!args || !(*args))
	{
		ft_putstr_fd("minishell : command not found\n", 2);
		free_paths(args);
		exit(127);
	}*/
//	cmd_not_raw(args);
	if (!(is_raw_cmd(args[0], cmd)))
		is_cmd(args[0], cmd);
	 /*if (!(is_cmd(args[0], cmd, envp)))
	{
		ft_putstr_fd("pipex: ", 2);
		if (args[0])
			write(2, args[0], ft_strlen(args[0]));
		ft_putstr_fd(": command not found\n", 2);
		free_paths(args);
		exit(127);
	}*/
	return (args);
}
