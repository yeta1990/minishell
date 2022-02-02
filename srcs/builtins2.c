/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/02 23:03:16 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_data *data, int index_exp, int i)
{
	int	index_env;

	index_env = search_word(data->env, data->cmds[0]->cmd_complete[i]);
	if ((index_exp >= 0) && (index_env >= 0))
	{
		free(data->exp[index_exp]);
		data->exp[index_exp] = export_join(data->cmds[0]->cmd_complete[i]);
		free(data->env[index_env]);
		data->env[index_env] = ft_strdup(data->cmds[0]->cmd_complete[i]);
	}
	else if (index_exp >= 0)
	{
		free(data->exp[index_exp]);
		data->exp[index_exp] = export_join(data->cmds[0]->cmd_complete[i]);
		data->env = add_entry(data->env,
				ft_strdup(data->cmds[0]->cmd_complete[i]));
	}
	else
	{
		data->exp = add_entry(data->exp,
				ft_strdup(data->cmds[0]->cmd_complete[i]));
		data->env = add_entry(data->env,
				ft_strdup(data->cmds[0]->cmd_complete[i]));
	}
}

int	export_error_checker(t_data *data, char *cmd_complete, int unset)
{
	int	j;
	int	equals;

	j = 0;
	equals = 0;
	if (ft_strlen(cmd_complete) > 0 && ft_isalpha(cmd_complete[0]) == 0)
		return (export_error(cmd_complete, data));
	while (cmd_complete[j] == ' ')
		j++;
	if (j == (int) ft_strlen(cmd_complete))
		return (export_error(cmd_complete, data));
	j = 0;
	while (cmd_complete[j])
	{
		if (cmd_complete[j] == '=' && equals == 0 && unset == 0)
			equals = 1;
		if (equals == 0 && ft_isalnum(cmd_complete[j]) == 0)
			return (export_error(cmd_complete, data));
		j++;
	}
	return (0);
}

int	export_alone(t_data *data, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->cmd_complete[1])
	{
		while (data->exp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(data->exp[i++], 1);
			ft_putstr_fd("\n", 1);
		}
		data->last_code = 0;
		return (1);
	}
	return (0);
}

int	export_builtin(t_data *data, t_cmd *cmd)
{
	int		i;
	int		index_exp;
	int		return_code;

	if (export_alone(data, cmd) == 1)
		return (0);
	return_code = 0;
	i = 0;
	while (cmd->cmd_complete[++i])
	{
		if (export_error_checker(data, cmd->cmd_complete[i], 0))
			return_code = 1;
		else
		{
			index_exp = search_word(data->exp, cmd->cmd_complete[i]);
			if (ft_strchr(cmd->cmd_complete[i], '='))
				update_env(data, index_exp, i);
			else
				if (index_exp < 0)
					data->exp = add_entry(data->exp,
							ft_strdup(cmd->cmd_complete[i]));
		}
	}
	data->last_code = return_code;
	return (return_code);
}

int	unset_builtin(t_data *data, t_cmd *cmd)
{
	int		index_env;
	int		index_exp;
	int		i;
	int		return_code;

	return_code = 0;
	i = 1;
	while (cmd->cmd_complete[i])
	{
		if (export_error_checker(data, cmd->cmd_complete[i], 1))
			return_code = 1;
		else
		{
			index_env = search_word_del(data->env, cmd->cmd_complete[i]);
			index_exp = search_word_del(data->exp, cmd->cmd_complete[i]);
			if (index_exp)
				data->exp = del_entry(data->exp, index_exp);
			if (index_env)
				data->env = del_entry(data->env, index_env);
		}
		i++;
	}
	return (return_code);
}
