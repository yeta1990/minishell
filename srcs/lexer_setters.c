/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_setters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 18:22:04 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 13:06:31 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//equivalence table for add_redirection:
//file redirected to standard input. type 0
//heredoc. type 1
//standard output redirected to a file. type 2
//standard output redirected to a file in append mode. type 3

int	add_redirection(char *name, t_cmd *cmd, int type, t_data *data)
{
	t_files	*file;
	int		size;

	size = 1;
	file = malloc(sizeof(t_files));
	while (name && *name && *name == ' ')
	{
		size++;
		name++;
	}
	file->name = ft_strdup_space(name, &size, 0);
	if (ft_strlen(file->name) == 0)
		syntax_error(file->name, data);
	else if (is_valid_infile(file->name) == 0)
		syntax_error(file->name, data);
	file->next = 0;
	if (type == 0 || type == 2)
		file->append = 0;
	else if (type == 1 || type == 3)
		file->append = 1;
	if (type == 0 || type == 1)
		ft_lstadd_back_files(cmd->stdins, file);
	else if (type > 1)
		ft_lstadd_back_files(cmd->stdouts, file);
	return (size);
}

int	add_cmd(char *raw_cmd, t_cmd *parsed_instruction)
{
	int		size;
	char	*aux;
	char	*word;

	aux = 0;
	size = 0;
	if (parsed_instruction->cmd_and_its_flags == 0)
		parsed_instruction->cmd_and_its_flags
			= ft_strdup_space(raw_cmd, &size, 1);
	else
	{
		aux = ft_strjoin (parsed_instruction->cmd_and_its_flags, " ");
		free(parsed_instruction->cmd_and_its_flags);
		word = ft_strdup_space(raw_cmd, &size, 1);
		parsed_instruction->cmd_and_its_flags = ft_strjoin(aux, word);
		free(word);
		free(aux);
	}
	return (size);
}
