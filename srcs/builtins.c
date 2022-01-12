/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/12 12:30:19 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_word_del(char **arr, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j] && str[j])
		{
			if (str[j] != arr[i][j])
				break ;
			else if ((!str[j + 1] && (arr[i][j + 1] == '=')) || (!str[j + 1] && !arr[i][j + 1]))
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int	search_word(char **arr, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j] && str[j])
		{
			if (str[j] != arr[i][j])
				break ;
			else if (((str[j + 1] == '=') && (arr[i][j + 1] == '=')) || ((str[j + 1] == '=') && !arr[i][j + 1]) || (!str[j + 1] && (arr[i][j + 1] == '=')) || (!str[j + 1] && !arr[i][j + 1]))
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

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

void	check_builtins(t_data data, char *str, char **instructions, char ***env, char ***exp)
{
	char	*buf = NULL;
	char	**aux_env;
	char	**aux_exp;
	char	*old_pwd = NULL;
	int		i;
	
	aux_env = *env;
	aux_exp = *exp;
	if (ft_strcmp("pwd", data.cmds[0]->cmd_complete[0]) == 0)
	{
		buf = getcwd(NULL, MAXPATHLEN);
		printf("%s\n", buf);
		free(buf);
	}
	if (ft_strcmp("cd", data.cmds[0]->cmd_complete[0]) == 0)
	{
		old_pwd = getcwd(NULL, MAXPATHLEN);
		chdir(data.cmds[0]->cmd_complete[1]);
		buf = getcwd(NULL, MAXPATHLEN);
		printf("%s\n", buf);
		i = 0;
		while (*(aux_env + i))
		{
			if (aux_env[i][0] == 'P' && aux_env[i][1] == 'W' && aux_env[i][2] == 'D' && aux_env[i][3] == '=')
			{
				free(aux_env[i]);
				aux_env[i] = ft_strjoin("PWD=", buf);
			}
			else if (aux_env[i][0] == 'O' && aux_env[i][1] == 'L' && aux_env[i][2] == 'D' && aux_env[i][3] == 'P' && aux_env[i][4] == 'W' && aux_env[i][5] == 'D' && aux_env[i][6] == '=')
			{
				free(aux_env[i]);
				aux_env[i] = ft_strjoin("OLDPWD=", old_pwd);
			}
			i++;
		}
		free(old_pwd);
		free(buf);
	}
	if ((ft_strcmp("env", data.cmds[0]->cmd_complete[0]) == 0) && (!data.cmds[0]->cmd_complete[1]))
	{
		i = 0;
		while (*(aux_env + i))
		{
			printf("%s\n", *(aux_env + i));
			i++;
		}
	}
	if ((ft_strcmp("export", data.cmds[0]->cmd_complete[0]) == 0) && (!data.cmds[0]->cmd_complete[1]))
	{
		i = 0;
		while (*(aux_exp + i))
		{
			printf("declare -x %s\n", *(aux_exp + i));
			i++;
		}
	}
	else if (ft_strcmp("export", data.cmds[0]->cmd_complete[0]) == 0)
	{
		int		index_env;
		int		index_exp;
		char	*sign;
		sign = 0;
		index_env = 0;
		index_exp = 0;
		i = 1;
		while (data.cmds[0]->cmd_complete[i])
		{
			sign = ft_strchr(data.cmds[0]->cmd_complete[i], '=');
			index_env = search_word(aux_env, data.cmds[0]->cmd_complete[i]);
			index_exp = search_word(aux_exp, data.cmds[0]->cmd_complete[i]);
			if (sign)
			{
				if ((index_exp >=0) && (index_env >=0))
				{
					free(aux_exp[index_exp]);
					aux_exp[index_exp] = export_join(data.cmds[0]->cmd_complete[i]);
					free(aux_env[index_env]);
					aux_env[index_env] = ft_strdup(data.cmds[0]->cmd_complete[i]);
				}
				else if (index_exp >=0)
				{
					free(aux_exp[index_exp]);
					aux_exp[index_exp] = export_join(data.cmds[0]->cmd_complete[i]);
					*env = add_entry(aux_env, ft_strdup(data.cmds[0]->cmd_complete[i]));
				}
				else
				{
					*exp = add_entry(aux_exp, ft_strdup(data.cmds[0]->cmd_complete[i]));
					*env = add_entry(aux_env, ft_strdup(data.cmds[0]->cmd_complete[i]));
				}
			}
			else
			{
				if (index_exp < 0)
					*exp = add_entry(aux_exp, ft_strdup(data.cmds[0]->cmd_complete[i]));
			}
			i++;
		}
	}
	if (ft_strcmp("unset", data.cmds[0]->cmd_complete[0]) == 0)
	{
		int		index_env;
		int		index_exp;
		while (data.cmds[0]->cmd_complete[i])
		{
			index_env = search_word_del(aux_env, data.cmds[0]->cmd_complete[i]);
			index_exp = search_word_del(aux_exp, data.cmds[0]->cmd_complete[i]);
			if (index_exp >= 0)
				*exp = del_entry(aux_exp, index_exp);
			if (index_env >= 0)
				*env = del_entry(aux_env, index_env);
			i++;
		}
	}
	if (ft_strcmp("echo", data.cmds[0]->cmd_complete[0]) == 0)
	{
		char	*aux;
		aux = data.cmds[0]->cmd_and_its_flags + 4;
		aux = echo_flag(aux);
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

