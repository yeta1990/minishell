/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 21:49:57 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 21:50:10 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
