/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:08:10 by crisfern          #+#    #+#             */
/*   Updated: 2022/02/01 16:31:30 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inc_shell_level(void)
{
	char	*str;
	int		num;
	int		i;

	i = search_word(data.env, "SHLVL");
	str = ft_strchr(data.env[i], '=');
	num = ft_atoi(str + 1) + 1;
	free(data.env[i]);
	data.env[i] = ft_strjoin("SHLVL=", ft_itoa(num));
	i = search_word(data.exp, "SHLVL");
	str = ft_strchr(data.exp[i], '=');
	free(data.exp[i]);
	str = ft_strjoin("SHLVL=\"", ft_itoa(num));
	data.exp[i] = ft_strjoin(str, "\"");
	free(str);
}
