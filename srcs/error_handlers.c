/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 10:06:51 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 11:23:07 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// other errors to take into account
// in bash, when "< /dev/random cat", ctrl-C, exit code 130
// in bash, when pressing ctrl-C, exit code 1
// segfault, 134
int	transform_error_code(char *cmd, int err)
{
	if (err == 13 && access(cmd, F_OK) != 0)
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
	else
	{
		perror("minishell");
		return (1);
	}
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

void	std_error(int errn)
{
	char	*msg;

	msg = strerror(errn);
	write(2, "minishell: ", 11);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

int	isalnum_string(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (isalnum(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_infile(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '<' || str[i] == '>')
			return (0);
		i++;
	}
	return (1);
}


void	syntax_error(char *wrong_portion, t_data *data)
{
	char	*msg;

	msg = ft_strdup("syntax error near unexpected token `");
	ft_putstr_fd("minishell: ", 2);
	write(2, msg, ft_strlen(msg));
	if (ft_strlen(wrong_portion) > 0)
		write(2, &wrong_portion[0], 1);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	data->syntax_error = 1;
	data->last_code = 258;
	free(msg);
}

int	parse_check(char *str)
{
	int		i;

	i = 0;
	if (!str || ft_strlen(str) == 0)
		return (0);
	while (str && str[i] && str[i] == ' ')
		str++;
	if (i < (int) ft_strlen(str))
		return (1);
	return (0);
}
