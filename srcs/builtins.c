/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/14 13:02:30 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_builtins(t_data data, char *str, char **instructions, char ***env, char ***exp)
{
	char	*buf1 = NULL;
	char	*buf2 = NULL;
	char	**aux_env;
	char	**aux_exp;
	int		i;
	
	aux_env = *env;
	aux_exp = *exp;
	if (ft_strcmp("pwd", data.cmds[0]->cmd_complete[0]) == 0)
	{
		buf2 = getcwd(buf1, MAXPATHLEN);
		printf("%s\n", buf2);
		free(buf1);
		free(buf2);
	}
	if (ft_strcmp("cd", data.cmds[0]->cmd_complete[0]) == 0)
	{
		chdir(data.cmds[0]->cmd_complete[1]);
	}
	if (ft_strcmp("env", data.cmds[0]->cmd_complete[0]) == 0)
	{
		i = 0;
		while (*(aux_env + i))
		{
			printf("%s\n", *(aux_env + i));
			i++;
		}
	}
	if (ft_strcmp("export", data.cmds[0]->cmd_complete[0]) == 0)
	{
		i = 0;
		while (*(aux_exp + i))
		{
			printf("declare -x %s\n", *(aux_exp + i));
			i++;
		}
	}
	if (ft_strcmp("echo -n", data.cmds[0]->cmd_complete[0]) == 0)
	{
		printf("HOLA");
	}
	if (ft_strcmp("exit", data.cmds[0]->cmd_complete[0]) == 0)
	{
		free_double_string(instructions);
		ft_bzero(str, ft_strlen(str));
		free(str);
		free_data(&data);
		exit(0);
	}
}
