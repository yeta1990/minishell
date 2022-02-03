/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 18:57:16 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 18:57:27 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_go_home(t_data *data, char **old_pwd, t_cmd *cmd)
{
	char	*home_dir;
	char	*home;

	if (!cmd->cmd_complete[1])
	{
		home = "$HOME";
		home_dir = expansor(&home, 0, data);
		if (ft_strlen(home_dir) == 0)
		{
			free(*old_pwd);
			free(home_dir);
			return (cd_error(0, -2, data));
		}
		else
			chdir(home_dir);
		free(home_dir);
		return (2);
	}
	return (0);
}

void	cd_set_new_vars(t_data *data, char **buf, char **old_pwd)
{
	int	i;

	i = -1;
	while (data->env[++i])
	{
		if (ft_strncmp("PWD=", data->env[i], 4) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("PWD=", *buf);
		}
		else if (ft_strncmp("OLDPWD=", data->env[i], 7) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("OLDPWD=", *old_pwd);
		}
	}
	free(*old_pwd);
	free(*buf);
}

int	cd_bultin(t_data *data, t_cmd *cmd)
{
	char	*old_pwd;
	char	*buf;
	int		i;

	old_pwd = getcwd(NULL, MAXPATHLEN);
	if (cd_go_home(data, &old_pwd, cmd) == 1)
		return (1);
	else if (cmd->cmd_complete[1])
	{
		i = chdir(cmd->cmd_complete[1]);
		if (i == -1)
		{
			free(old_pwd);
			return (cd_error(cmd->cmd_complete[1], errno, data));
		}
	}
	buf = getcwd(NULL, MAXPATHLEN);
	if (!buf)
		cd_error("error retrieving current directory: getcwd: cannot access parent directories", errno, data);
	cd_set_new_vars(data, &buf, &old_pwd);
	return (0);
}
