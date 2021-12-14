/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/14 12:29:46 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_env_size(char **envp)
{
	int	size;

	size = 0;
	while (*envp)
	{
		envp++;
		size++;
	}
	return (size);
}

char	**create_env(char **envp)
{
	int		i;
	int		size;
	char	**env;

	i = 0;
	size = get_env_size(envp);
	env = ft_calloc(size + 1, sizeof(char *));
	while (*envp)
	{
		env[i] = ft_strdup(*envp);
		envp++;
		i++;
	}
	return (env);
}

static char	*export_join(char *str)
{
	char	*first;
	char	*last;
	char	*aux1;
	char	*aux2;
	char	*aux3;

	aux1 = ft_strchr(str, '=');
	last = ft_strdup(aux1 + 1);
	first = ft_substr(str, 0, aux1 + 1 - str);
	aux1 = ft_strjoin(first, "\"");
	aux2 = ft_strjoin(last, "\"");
	free(first);
	free(last);
	aux3 = ft_strjoin(aux1, aux2);
	free(aux1);
	free(aux2);
	return (aux3);
}

char	**create_exp(char **envp)
{
	int		i;
	int		size;
	char	**exp;

	i = 0;
	size = get_env_size(envp);
	exp = ft_calloc(size + 1, sizeof(char *));
	while (*envp)
	{
		exp[i] = export_join(*envp);
		envp++;
		i++;
	}
	return (exp);
}
