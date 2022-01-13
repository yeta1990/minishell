/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:09:51 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/13 10:41:35 by albgarci         ###   ########.fr       */
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

void	reset_data(t_data *data)
{
	free(data->cmds);
	data->cmds = 0;
	data->num_cmds = 0;
	data->num_pipes = 0;
}

void	free_data(t_data *data)
{
	t_cmd	*aux;

	while (*data->cmds)
	{
		aux = *data->cmds;
		*data->cmds = aux->next;
		free(aux->cmd);
		ft_bzero(aux->cmd_and_its_flags, ft_strlen(aux->cmd_and_its_flags));
		free(aux->cmd_and_its_flags);
		free_files(*aux->stdins);
		free(aux->stdins);
		aux->stdins = 0;
		free_files(*aux->stdouts);
		free(aux->stdouts);
		aux->stdouts = 0;
		free_files(*aux->stderrs);
		free(aux->stderrs);
		aux->stderrs = 0;
		free_files(*aux->heredocs);
		free(aux->heredocs);
		aux->heredocs = 0;
		aux->next = 0;
		free(aux);
		free_double_string(aux->cmd_complete);
	}
}

void	free_files(t_files *first)
{
	t_files	*aux;

	while (first)
	{
		aux = first;
		first = aux->next;
		ft_bzero(aux->name, ft_strlen(aux->name));
		free(aux->name);
		free(aux);
	}
}
