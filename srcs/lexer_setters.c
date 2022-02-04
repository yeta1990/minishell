/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_setters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 18:22:04 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/04 12:05:01 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//equivalence table for add_redirection:
//file redirected to standard input. type 0
//heredoc. type 1
//standard output redirected to a file. type 2
//standard output redirected to a file in append mode. type 3

void	set_filename(char *name, t_data *data, t_files *file, int *size)
{
	char	*piece;
	char	*trimmed;

	piece = ft_strdup_space(name, size, 0);
	trimmed = ft_strtrim(piece, "\"");
	file->name = expansor(&trimmed, 0, data);
	free(piece);
	free(trimmed);
}

int	add_redirection(char *name, t_cmd *cmd, int type, t_data *data)
{
	t_files	*file;
	int		size;

	size = 1;
	file = malloc(sizeof(t_files));
	while (name && *name && (*name == ' ' || *name == 9))
	{
		size++;
		name++;
	}
	set_filename(name, data, file, &size);
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

void	expand_cmd(char *raw_cmd, t_cmd *parsed_instr, t_data *data, int *size)
{
	char	*expanded;
	char	*false_dup;
	int		i;
	char	*s;

	i = 0;
	expanded = 0;
	false_dup = ft_strdup_space(raw_cmd, size, 1);
	free(false_dup);
	while (i < (int) ft_strlen(raw_cmd) && raw_cmd[i] != ' ' && raw_cmd[i] != 9)
		i++;
	s = ft_substr(raw_cmd, 0, i);
	expanded = expansor(&s, 0, data);
	parsed_instr->cmd_and_its_flags = ft_strdup(expanded);
	free(expanded);
	free(s);
}

int	add_cmd(char *raw_cmd, t_cmd *pars_instr, t_data *data)
{
	int		size;
	char	*aux;
	char	*word;

	size = 0;
	if (pars_instr->cmd_and_its_flags == 0)
	{
		if (ft_strlen(raw_cmd) > 1 && *raw_cmd != '\''
			&& ft_strchr(raw_cmd, '$') != 0)
		{
			expand_cmd(raw_cmd, pars_instr, data, &size);
		}
		else
			pars_instr->cmd_and_its_flags = ft_strdup_space(raw_cmd, &size, 1);
	}
	else
	{
		aux = ft_strjoin (pars_instr->cmd_and_its_flags, " ");
		free(pars_instr->cmd_and_its_flags);
		word = ft_strdup_space(raw_cmd, &size, 1);
		pars_instr->cmd_and_its_flags = ft_strjoin(aux, word);
		free(word);
		free(aux);
	}
	return (size);
}
