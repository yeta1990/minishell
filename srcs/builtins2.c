/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/13 13:47:39 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*echo_flag(char *str)
{
	char	*init;
	int		flag;

	flag = 0;
	if (str)
	{
		while (*str)
		{
			while (*str == ' ')
				str++;
			init = str;
			if (*str++ != '-')
				break ;
			while (*str == 'n')
				str++;
			if ((*str != '\0') && (*str != ' '))
				break ;
			flag = 1;
		}
		if (flag)
			return (ft_strdup(init));
		else
			return (ft_strjoin(init, "\n"));
	}
	return (0);
}

void	echo_builtin(t_data *data)
{
	char	*aux;

	aux = data->cmds[0]->cmd_and_its_flags + 4;
	aux = echo_flag(aux);
	ft_putstr_fd(aux, 1);
	free(aux);
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
			data->env = del_entry(data->env, index_exp);
		i++;
	}
}
