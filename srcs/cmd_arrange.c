/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arrange.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:44:06 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/18 15:36:28 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_raw_cmd(char *file, char **cmd_ok)
{
	int		pos;
	char	*aux;

	pos = ft_strlen(file) - 1;
	if (access(file, X_OK) != -1)
	{
		if (cmd_ok)
			*cmd_ok = ft_strdup(file);
		while (pos >= 0)
		{
			if (file[pos] == '/')
			{
				aux = ft_substr(file, pos, ft_strlen(file) - pos + 1);
				free(file);
				file = aux;
			}
			pos--;
		}
		return (1);
	}
	return (0);
}

int	is_cmd(char *file, char **cmd_ok)
{
	int		j;
	char	*cmd_try;
	char	**paths;

	j = 0;
	paths = get_paths(getenv("PATH"));
	while (paths[j])
	{
		cmd_try = ft_strjoin(paths[j], file);
		if (access(cmd_try, X_OK) != -1)
		{
			if (cmd_ok)
				*cmd_ok = ft_strdup(cmd_try);
			free(cmd_try);
			free_paths(paths);
			return (1);
		}
		free(cmd_try);
		j++;
	}
	free_paths(paths);
	*cmd_ok = 0;
	if (file)
		*cmd_ok = ft_strdup(file);
	return (0);
}

char	**create_args(char *raw_cmd, char **cmd, t_data *data)
{
	char	**args;
	char	*aux;
	int		i;

	i = 0;
//	printf("raw cmd: %s\n", raw_cmd);
	args = split_quote_sensitive(raw_cmd);
	if (args)
	{
		while (args && args[i])
		{
		//	printf("a\n");
		//	aux = args[i];
			aux = ft_strtrim(args[i], "\"");
			//free(aux);
		//	if (ft_strlen(aux) == 0)
		//		aux = ft_strdup(" ");// <- leaks
			free(args[i]);
			args[i] = expansor(&aux, data);
		//	printf("%s\n", args[i]);
			if (aux)
				free(aux);
			i++;
		}
	}
	if (!(is_raw_cmd(args[0], cmd)))
		is_cmd(args[0], cmd);
	return (args);
}

char	*expansor(char **arg, t_data *data)
{
	char	*a;
	char	*exp;
	char	*aux_exp;
	char	*aux_exp2;
	char	*aux_exp3;
	int		i;
	int		j;

	data = data + 0;
	j = 0;
	i = 0;
	exp = 0;
	aux_exp = 0;
	aux_exp2 = 0;
	aux_exp3 = 0;
	a = *arg;
	printf("to expand->%s\n", a);
	while (a && a[i] && a[i] != '$')
		i++;
	if (i != 0)
		exp = ft_substr(a, 0, i);
	i++;
	while (a && a[i])
	{
		while (a[i] == '$')
			i++;
		if (a[i] && a[i] == '?')
		{
			aux_exp3 = ft_itoa(data->last_code);
			j++;
		}
		else
		{
			while (a[i + j] && a[i + j] != '$')
				j++;
			aux_exp = ft_substr(a, i, j);
			aux_exp3 = ft_strdup(getenv(aux_exp));
			if (aux_exp)
				free(aux_exp);
		}
		if (exp)
			aux_exp2 = ft_strjoin(exp, aux_exp3);
		else if (aux_exp3)
			aux_exp2 = ft_strdup(aux_exp3);
		free(aux_exp3);
		if (aux_exp2)
		{
			if (exp)
				free(exp);
			exp = ft_strdup(aux_exp2);
			free(aux_exp2);
		}
		i += j;
		j = 0;
	}
	return (exp);
}
