/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:58:15 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/03 18:02:36 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_final_condition(t_data *data, int *code)
{
	if (data->num_cmds == 1)
	{
		ft_putstr_fd("exit\n", 1);
		exit(*code);
	}
	return (*code);
}

int	exit_builtin(t_data *data, t_cmd *cmd)
{
	int	code;

	code = 0;
	if (cmd->cmd_complete[1])
		code = ft_atoi(cmd->cmd_complete[1]);
	data->last_code = code;
	if (cmd->cmd && ft_strcmp("exit", cmd->cmd_complete[0]) != 0)
		return (code);
	if (is_not_numeric(cmd->cmd_complete[1]))
	{
		exit_errors(cmd->cmd_complete[1], 2);
		code = 255;
		if (data->num_cmds > 1)
			exit(code);
	}
	else if (cmd->cmd_complete[1] && cmd->cmd_complete[2])
	{
		data->last_code = 1;
		exit_errors(0, 1);
		return (1);
	}
	return (exit_final_condition(data, &code));
}
