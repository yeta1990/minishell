/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_from_user.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 19:24:47 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 13:10:45 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_from_user(t_data *data)
{
	char	*str;
	int		i;
	int		j;

	signal(SIGINT, SIG_DFL);
	str = readline("> ");
	while (str)
	{
		i = 0;
		j = 0;
		while (str && str[i])
		{
			if (str[i] == ' ')
				j++;
			i++;
		}
		if (i != j && ft_strlen(str) > 0)
			break ;
		free(str);
		str = 0;
		str = readline("> ");
	}
	data->cmd_by_stdin = 1;
	return (str);
}
