/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 11:46:47 by albgarci         ###   ########.fr       */
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

void	echo_builtin(t_cmd *cmd)
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

void	export_builtin(t_data *data)
{
	int		i;
	int		index_exp;
	
	i = 0;
	if (!data->cmds[0]->cmd_complete[1])
	{
		while (data->exp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(data->exp[i++], 1);
			ft_putstr_fd("\n", 1);
		}
		return ;
	}
	while (data->cmds[0]->cmd_complete[++i])
	{
		index_exp = search_word(data->exp, data->cmds[0]->cmd_complete[i]);
		if (ft_strchr(data->cmds[0]->cmd_complete[i], '='))
			update_env(data, index_exp, i);
		else
			if (index_exp < 0)
				data->exp = add_entry(data->exp,
						ft_strdup(data->cmds[0]->cmd_complete[i]));
	}
}

void	unset_builtin(t_data *data)
{
	int		index_env;
	int		index_exp;
	int		i;

	i = 1;
	while (data->cmds[0]->cmd_complete[i])
	{
		index_env = search_word_del(data->env, data->cmds[0]->cmd_complete[i]);
		index_exp = search_word_del(data->exp, data->cmds[0]->cmd_complete[i]);
		if (index_exp)
			data->exp = del_entry(data->exp, index_exp);
		if (index_env)
			data->env = del_entry(data->env, index_env);
		i++;
	}
	i = 0;
}
