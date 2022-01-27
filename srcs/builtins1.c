/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 17:39:38 by albgarci         ###   ########.fr       */
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

int	cd_bultin(t_data *data, t_cmd *cmd)
{
	char	*old_pwd;
	char	*buf;
	int		i;


	old_pwd = getcwd(NULL, MAXPATHLEN);
	i = chdir(cmd->cmd_complete[1]);
	if (i == -1)
	{
		free(old_pwd);
		return (cd_error(cmd->cmd_complete[1], errno, data));
	}
	buf = getcwd(NULL, MAXPATHLEN);
	i = -1;
	while (data->env[++i])
	{
		if (ft_strncmp("PWD=", data->env[i], 4) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("PWD=", buf);
		}
		else if (ft_strncmp("OLDPWD=", data->env[i], 7) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strjoin("OLDPWD=", old_pwd);
		}
	}
	free(old_pwd);
	free(buf);
	return (0);
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

int	exit_builtin(t_data *data, t_cmd *cmd)
{
	int	code;

	code = 0;
	if (cmd->cmd_complete[1])
		code = ft_atoi(cmd->cmd_complete[1]);
	data->last_code = code;
	if (cmd->cmd && ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
	{
		if (data->num_cmds == 1)
		{
			ft_putstr_fd("exit\n", 1);
			free_data(data);
			exit(code);
		}
	}
	return (code);
}

int	check_builtins(t_data *data, t_cmd *cmd)
{
	int	is_builting;

	is_builting = 1;
	if (ft_strcmp("cd", cmd->cmd_complete[0]) == 0)
		cd_bultin(data, cmd);
	else if (ft_strcmp("pwd", cmd->cmd_complete[0]) == 0)
		pwd_builtin();
	else if ((ft_strcmp("env", cmd->cmd_complete[0]) == 0)
		&& (!cmd->cmd_complete[1]))
		env_builtin(data);
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0 && !(cmd->cmd_complete[1]))
		export_builtin(data);
	else if (ft_strcmp("echo", cmd->cmd_complete[0]) == 0)
		echo_builtin(cmd);
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
		exit_builtin(data, cmd);
	else
		is_builting = 0;
	return (is_builting);
}

int	check_outside_builtins(t_data *data, t_cmd *cmd)
{
	int	is_builting;

	is_builting = 1;
	if (!cmd->cmd_complete[0])
		return (0);
	if (ft_strcmp("cd", cmd->cmd_complete[0]) == 0 && data->num_cmds == 1)
		return (cd_bultin(data, cmd));
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0 && cmd->cmd_complete[1])
		export_builtin(data);
	else if (ft_strcmp("unset", cmd->cmd_complete[0]) == 0)
		unset_builtin(data);
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
		exit_builtin(data, cmd);
	else
		is_builting = 0;
	return (-100);
}
