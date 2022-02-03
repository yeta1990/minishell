/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/03 14:04:03 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_builtin(t_data *data)
{
	char	*buf;

	buf = getcwd(NULL, MAXPATHLEN);
	if (!buf)
		return (std_error(errno));
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("\n", 1);
	data->last_code = 0;
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
	data->last_code = 0;
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
		if (cmd->cmd_complete[1] && cmd->cmd_complete[2])
		{
			exit_errors(0, 1);
			data->last_code = 1;
			return (1);
		}
		else if (is_not_numeric(cmd->cmd_complete[1]))
		{
			exit_errors(cmd->cmd_complete[1], 2);
			code = 255;
			data->last_code = code;
		}
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
		pwd_builtin(data);
	else if ((ft_strcmp("env", cmd->cmd_complete[0]) == 0)
		&& (!cmd->cmd_complete[1]))
		env_builtin(data);
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0)
		export_builtin(data, cmd);
	else if (ft_strcmp("unset", cmd->cmd_complete[0]) == 0)
		unset_builtin(data, cmd);
	else if (ft_strcmp("echo", cmd->cmd_complete[0]) == 0)
		echo_builtin(cmd, data);
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
	{
		if (cmd->cmd_complete[2])
			return (1);
		exit_builtin(data, cmd);
	}
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
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0
		&& data->num_cmds == 1)
		return (export_builtin(data, cmd));
	else if (ft_strcmp("unset", cmd->cmd_complete[0]) == 0
		&& data->num_cmds == 1)
		return (unset_builtin(data, cmd));
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0
		&& data->num_cmds == 1)
		exit_builtin(data, cmd);
	else
		is_builting = 0;
	return (-100);
}
