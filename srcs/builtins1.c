/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/13 13:14:23 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_builtin(void)
{
	char	*buf;

	buf = getcwd(NULL, MAXPATHLEN);
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("\n", 1);
	free(buf);
}

void	cd_bultin(t_data *data)
{
	char	*old_pwd;
	char	*buf;
	int		i;

	old_pwd = getcwd(NULL, MAXPATHLEN);
	chdir(data->cmds[0]->cmd_complete[1]);
	buf = getcwd(NULL, MAXPATHLEN);
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("\n", 1);
	i = -1;
	while (data->env[++i])
	{
		if (data->env[i][0] == 'P' && data->env[i][1] == 'W' && data->env[i][2] == 'D' && data->env[i][3] == '=')
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("PWD=", buf);
		}
		else if (data->env[i][0] == 'O' && data->env[i][1] == 'L' && data->env[i][2] == 'D' && data->env[i][3] == 'P' && data->env[i][4] == 'W' && data->env[i][5] == 'D' && data->env[i][6] == '=')
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("OLDPWD=", old_pwd);
		}
	}
	free(old_pwd);
	free(buf);
}

void	env_builtin(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		ft_putstr_fd(data->env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

void	exit_builtin(t_data *data, char *str, char **instructions)
{
	free_double_string(instructions);
	ft_bzero(str, ft_strlen(str));
	free(str);
	free_data(data);
	exit(0);
}

int	check_builtins(t_data *data, char *str, char **instructions)
{
	int	is_builting;

	is_builting = 1;
	if (ft_strcmp("pwd", data->cmds[0]->cmd_complete[0]) == 0)
		pwd_builtin();
	else if (ft_strcmp("cd", data->cmds[0]->cmd_complete[0]) == 0)
		cd_bultin(data);
	else if ((ft_strcmp("env", data->cmds[0]->cmd_complete[0]) == 0)
		&& (!data->cmds[0]->cmd_complete[1]))
		env_builtin(data);
	else if (ft_strcmp("export", data->cmds[0]->cmd_complete[0]) == 0)
		export_builtin(data);
	else if (ft_strcmp("unset", data->cmds[0]->cmd_complete[0]) == 0)
		unset_builtin(data);
	else if (ft_strcmp("echo", data->cmds[0]->cmd_complete[0]) == 0)
		echo_builtin(data);
	else if (ft_strcmp("exit", data->cmds[0]->cmd_complete[0]) == 0)
		exit_builtin(data, str, instructions);
	else
		is_builting = 0;
	return (is_builting);
}