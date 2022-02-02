/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 10:06:51 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/02 22:54:09 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	transform_error_code(char *cmd, int err)
{
	if (err == 13 && access(cmd, F_OK) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 20);
		return (126);
	}
	else if (err == 2 || err == 13)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		if (ft_strchr(cmd, '/') == 0 || err == 13)
			write(2, ": command not found\n", 20);
		else
			write(2, ": No such file or directory\n", 28);
		return (127);
	}
	else if (err == 11)
	{
		write(2, "Segmentation fault: 11\n", 23);
		return (139);
	}
	perror("minishell");
	return (1);
}

int	cd_error(char *filename, int errn, t_data *data)
{
	ft_putstr_fd("minishell: cd: ", 2);
	write(2, filename, ft_strlen(filename));
	if (errn == -2)
		ft_putstr_fd("HOME not set", 2);
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errn), 2);
	}
	ft_putstr_fd("\n", 2);
	data->last_code = 1;
	return (1);
}

int	export_error(char *filename, t_data *data)
{
	ft_putstr_fd("minishell: export: `", 2);
	write(2, filename, ft_strlen(filename));
	ft_putstr_fd("': not a valid identifier\n", 2);
	data->last_code = 1;
	return (1);
}

void	file_error(char *filename, int errn)
{
	ft_putstr_fd("minishell: ", 2);
	write(2, filename, ft_strlen(filename));
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errn), 2);
	ft_putstr_fd("\n", 2);
	exit(errn);
}
