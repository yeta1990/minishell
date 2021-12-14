/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:09:51 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/14 12:30:42 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_string(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		ft_bzero(str[i], ft_strlen(str[i]));
		free(str[i]);
		i++;
	}
	free(str);
	str = 0;
}

void	free_data(t_data *data)
{
	t_cmd *cmd_aux;

	while (*data->cmds)
	{
		cmd_aux = *data->cmds;
		*data->cmds = cmd_aux->next;
		free(cmd_aux->cmd);
		ft_bzero(cmd_aux->cmd_and_its_flags, ft_strlen(cmd_aux->cmd_and_its_flags));
		free(cmd_aux->cmd_and_its_flags);
		free_files(*cmd_aux->stdins);
		free(cmd_aux->stdins);
		cmd_aux->stdins = 0;
		free_files(*cmd_aux->stdouts);
		free(cmd_aux->stdouts);
		cmd_aux->stdouts = 0;
		free_files(*cmd_aux->stderrs);
		free(cmd_aux->stderrs);
		cmd_aux->stderrs = 0;
		free_files(*cmd_aux->heredocs);
		free(cmd_aux->heredocs);
		cmd_aux->heredocs = 0;
		free(cmd_aux);
		free_double_string(cmd_aux->cmd_complete);
	}
	free(data->cmds);
	data->cmds = 0;
	data->num_cmds = 0;
	data->num_pipes = 0;
}

void	free_files(t_files *first)
{
	t_files *aux;

	while (first)
	{
		aux = first;
		first = aux->next;
		ft_bzero(aux->name, ft_strlen(aux->name));
		free(aux->name);
		free(aux);
	}
}
