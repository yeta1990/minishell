/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 10:06:51 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/13 10:34:42 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// other errors to take into account
// in bash, when "< /dev/random cat", ctrl-C, exit code 130
// in bash, when pressing ctrl-C, exit code 1
// segfault, 134
int	transform_error_code(char *cmd, int err)
{
	if (err == 13)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 20);
		return (126);
	}
	else if (err == 2)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20);
		return (127);
	}
	else
	{
		perror("minishell");
		return (1);
	}
}

void	std_error(int errn)
{
	char	*msg;

	msg = strerror(errn);
	write(2, "minishell: ", 11);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
