/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:09:51 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/09 13:20:11 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_string(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_data(t_data *data)
{
	t_cmd *cmd_aux;


	while (*data->cmds)
	{
		free((*data->cmds)->cmd);
		free_double_string((*data->cmds)->cmd_complete);
		free((*data->cmds)->cmd_and_its_flags);
		free_files(*(*data->cmds)->stdins);
		free((*data->cmds)->stdins);
		free_files(*(*data->cmds)->stdouts);
		free((*data->cmds)->stdouts);
		free_files(*(*data->cmds)->stderrs);
		free((*data->cmds)->stderrs);
		free_files(*(*data->cmds)->heredocs);
		free((*data->cmds)->heredocs);
		cmd_aux = *data->cmds;
		*data->cmds = cmd_aux->next;
		free(cmd_aux);
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
		printf("cleaning \n");
		aux = first;
		first = aux->next;
		free(aux->name);
		free(aux);
	}
}
