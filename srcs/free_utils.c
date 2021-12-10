/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:09:51 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/10 10:44:05 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_string(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
	//	printf("cleaning %s\n", str[i]);
		free(str[i]);
		i++;
	}
//	free(str[i]);
	free(str);
}

void	free_data(t_data *data)
{
	t_cmd *cmd_aux;

	while (*data->cmds)
	{
		cmd_aux = *data->cmds;
		*data->cmds = cmd_aux->next;
		free(cmd_aux->cmd);
		free(cmd_aux->cmd_and_its_flags);
		free_files(*cmd_aux->stdins);
		free(cmd_aux->stdins);
		free_files(*cmd_aux->stdouts);
		free(cmd_aux->stdouts);
		free_files(*cmd_aux->stderrs);
		free(cmd_aux->stderrs);
		free_files(*cmd_aux->heredocs);
		free(cmd_aux->heredocs);
		free(cmd_aux);
		free_double_string(cmd_aux->cmd_complete);
	}
	free(data->cmds);
	data->num_cmds = 0;
	data->num_pipes = 0;
}

void	free_files(t_files *first)
{
	t_files *aux;

	while (first)
	{
	//	printf("cleaning %s\n", first->name);
		aux = first;
		first = aux->next;
		free(aux->name);
		free(aux);
	}
}
