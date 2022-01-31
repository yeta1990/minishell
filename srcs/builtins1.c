/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/26 09:29:44 by crisfern         ###   ########.fr       */
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

int	cd_bultin(t_data *data)
{
	char	*old_pwd;
	char	*buf;
	int		i;

	old_pwd = getcwd(NULL, MAXPATHLEN);
	if(chdir(data->cmds[0]->cmd_complete[1]) == -1)
	{
		buf = ft_strjoin("cd: ", data->cmds[0]->cmd_complete[1]);
		//transform_error_code(data->cmds[0]->cmd_complete[1], 2);
		std_error(2);
		free(buf);
		free(old_pwd);
		return (1);
	}
	buf = getcwd(NULL, MAXPATHLEN);
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
	i = -1;
	while (data->exp[++i])
	{
		if (data->exp[i][0] == 'P' && data->exp[i][1] == 'W' && data->exp[i][2] == 'D' && data->exp[i][3] == '=')
		{
			free(data->exp[i]);
			data->exp[i] = ft_strjoin("PWD=", buf);
		}
		else if (data->exp[i][0] == 'O' && data->exp[i][1] == 'L' && data->exp[i][2] == 'D' && data->exp[i][3] == 'P' && data->exp[i][4] == 'W' && data->exp[i][5] == 'D' && data->exp[i][6] == '=')
		{
			free(data->exp[i]);
			data->exp[i] = ft_strjoin("OLDPWD=", old_pwd);
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
	if (ft_strcmp("pwd", cmd->cmd_complete[0]) == 0)
		pwd_builtin(data);
	else if ((ft_strcmp("env", cmd->cmd_complete[0]) == 0)
		&& (!cmd->cmd_complete[1]))
		env_builtin(data);
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0 && !(cmd->cmd_complete[1]))
		export_builtin(data);
	else if (ft_strcmp("echo", cmd->cmd_complete[0]) == 0)
		echo_builtin(data, cmd);
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
		exit_builtin(data, cmd);
	else
		is_builting = 0;
	return (is_builting);
}

int	check_outside_builtins(t_data *data, t_cmd *cmd)
{
	if (ft_strcmp("cd", cmd->cmd_complete[0]) == 0)
		return (cd_bultin(data));
	else if (ft_strcmp("export", cmd->cmd_complete[0]) == 0 && cmd->cmd_complete[1])
		return (export_builtin(data));
	else if (ft_strcmp("unset", cmd->cmd_complete[0]) == 0)
		return (unset_builtin(data));
	else if (ft_strcmp("exit", cmd->cmd_complete[0]) == 0)
		return (exit_builtin(data, cmd));
	return (-1);
}
