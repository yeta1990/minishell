/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 18:22:18 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 18:42:04 by albgarci         ###   ########.fr       */
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

char	*echo_init(t_cmd *cmd, int *i, int *new_line)
{
	char	*aux;

	aux = 0;
	if (cmd->cmd_complete && cmd->cmd_complete[0])
	{
		*i = echo_flag(&(cmd->cmd_complete[1])) + 1;
		if (*i > 1)
			*new_line = 0;
		aux = ft_strdup(cmd->cmd_complete[*i]);
	}
	else
		aux = ft_strdup("");
	return (aux);
}

void	echo_builtin(t_cmd *cmd, t_data *data)
{
	char	*aux;
	int		i;
	int		new_line;
	int		first_space;

	first_space = 0;
	i = 0;
	new_line = 1;
	aux = echo_init(cmd, &i, &new_line);
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
