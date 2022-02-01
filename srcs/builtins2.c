/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 17:16:43 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_flag(char **cmd_complete)
{
	int		flags;
	char	*aux;

	flags = 0;
	aux = cmd_complete[flags];
	while (aux)
	{
		if (*aux && *aux != '-')
			return (flags);
		aux++;
		if (*aux && *aux == 'n')
			aux++;
		else
			return (flags);
		while (*aux && *aux == 'n')
			aux++;
		if (*aux)
			return (flags);
		flags++;
		aux = cmd_complete[flags];
	}
	return (flags);
}

void	echo_builtin(t_cmd *cmd, t_data *data)
{
	char	*aux;
	int		i;
	int		new_line;
	int		first_space;

	first_space = 0;
	i = 0;
	aux = 0;
	new_line = 1;
	if (cmd->cmd_complete && cmd->cmd_complete[0])
	{
		i = echo_flag(&(cmd->cmd_complete[1])) + 1;
		if (i > 1)
			new_line = 0;
		aux = ft_strdup(cmd->cmd_complete[i]);
	}
	else
		aux = ft_strdup("");
	first_space = i;
	while (aux)
	{
		if ((i >= first_space + 1 && new_line == 1) || i > first_space)
			write(1, " ", 1);
		ft_putstr_fd(aux, 1);
		free(aux);
		i++;
		aux = ft_strdup(cmd->cmd_complete[i]);
	}
	free(aux);
	if (new_line)
		write(1, "\n", 1);
	data->last_code = 0;
}

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

//export errors
//si empieza por no alpha, error
//si contiene no alphanum, error
//si contiene un '$' (es decir, que el expander haya dejado la '$' tal cual, también error

int	export_error_checker(t_data *data, t_cmd *cmd)
{
	int	i;
	int	j;
	int	equals;

	i = 1;
	equals = 0;
	while (cmd->cmd_complete[i])
	{
		j = 0;
		if (ft_strlen(cmd->cmd_complete[i]) > 0 && ft_isalpha(cmd->cmd_complete[i][0]) == 0)
			return (export_error(cmd->cmd_complete[i], data));
		while (cmd->cmd_complete[i][j])
		{
			if (cmd->cmd_complete[i][j] == '=' && equals == 0)
				equals = 1;
			if (equals == 0 && ft_isalnum(cmd->cmd_complete[i][j]) == 0)
				return (export_error(cmd->cmd_complete[i], data));
			j++;
		}
		i++;
	}
	return (0);
}

int	export_builtin(t_data *data, t_cmd *cmd)
{
	int		i;
	int		index_exp;
	
	i = 0;
	if (!cmd->cmd_complete[1] || (cmd->cmd_complete[1] && ft_strlen(cmd->cmd_complete[1]) == 0))
	{
		while (data->exp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(data->exp[i++], 1);
			ft_putstr_fd("\n", 1);
		}
		data->last_code = 0;
		return (0);
	}
	if (export_error_checker(data, cmd))
		return (1);
	while (cmd->cmd_complete[++i])
	{
		index_exp = search_word(data->exp, cmd->cmd_complete[i]);
		if (ft_strchr(cmd->cmd_complete[i], '='))
			update_env(data, index_exp, i);
		else
			if (index_exp < 0)
				data->exp = add_entry(data->exp,
						ft_strdup(cmd->cmd_complete[i]));
	}
	data->last_code = 0;
	return (0);
}

int	unset_builtin(t_data *data, t_cmd *cmd)
{
	int		index_env;
	int		index_exp;
	int		i;

	i = 1;
	if (export_error_checker(data, cmd))
		return (1);
	while (cmd->cmd_complete[i])
	{
		index_env = search_word_del(data->env, cmd->cmd_complete[i]);
		index_exp = search_word_del(data->exp, cmd->cmd_complete[i]);
		if (index_exp)
			data->exp = del_entry(data->exp, index_exp);
		if (index_env)
			data->env = del_entry(data->env, index_env);
		i++;
	}
	return (0);
}
