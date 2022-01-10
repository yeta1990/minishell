/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/10 17:19:25 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*is_echo_flag(char *str)
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
		buf2 = getcwd(buf1, MAXPATHLEN);
		free(buf2);
		i = 0;
		while (*(aux_env + i))
		{
			if (aux_env[i][0] == 'P' && aux_env[i][1] == 'W' && aux_env[i][2] == 'D' && aux_env[i][3] == '=')
			{
				free(aux_env[i]);
				aux_env[i] = buf1;
				printf("La cadena %s\n", aux_env[i]);
				break ;
			}
			i++;
		}
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
	if (ft_strcmp("echo", data.cmds[0]->cmd_complete[0]) == 0)
	{
		char	*aux;
		aux = data.cmds[0]->cmd_and_its_flags + 4;
		aux = is_echo_flag(aux);
		printf("%s", aux);
		free(aux); 
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
