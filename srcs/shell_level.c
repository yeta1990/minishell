/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:08:10 by crisfern          #+#    #+#             */
/*   Updated: 2022/02/03 16:17:09 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inc_shell_level(void)
{
	char	*str;
	int		num;
	int		i;
	char	*num_string;

	num_string = 0;
	i = search_word(g_data.env, "SHLVL");
	str = ft_strchr(g_data.env[i], '=');
	num = ft_atoi(str + 1) + 1;
	free(g_data.env[i]);
	num_string = ft_itoa(num);
	g_data.env[i] = ft_strjoin("SHLVL=", num_string);
	i = search_word(g_data.exp, "SHLVL");
	str = ft_strchr(g_data.exp[i], '=');
	free(g_data.exp[i]);
	str = ft_strjoin("SHLVL=\"", num_string);
	free(num_string);
	g_data.exp[i] = ft_strjoin(str, "\"");
	free(str);
}
