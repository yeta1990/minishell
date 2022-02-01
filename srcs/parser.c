/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:25:15 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 13:03:56 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_infiles(t_data *data, t_cmd *parsed_instruction, char **str)
{
	(*str)++;
	if (str && *str && **str && **str == '<')
	{
		(*str)++;
		*str += add_redirection(*str, parsed_instruction, 1, data);
	}
	else
		*str += add_redirection(*str, parsed_instruction, 0, data) - 1;
}

void	parse_outfiles(t_data *data, t_cmd *parsed_instruction, char **str)
{
	(*str)++;
	if (str && *str && **str && **str == '>')
	{
		(*str)++;
		*str += add_redirection(*str, parsed_instruction, 3, data);
	}
	else
		*str += add_redirection(*str, parsed_instruction, 2, data) - 1;
}

void	parse_instruction(char *s, t_cmd *parsed_instruction, t_data *data)
{
	char	*str;

	str = s;
	while (str && *str && data->syntax_error == 0)
	{
		while (*str && *str == ' ')
			str++;
		if (*str == '<')
			parse_infiles(data, parsed_instruction, &str);
		else if (*str == '>')
			parse_outfiles(data, parsed_instruction, &str);
		else if (*str != '<' && *str != '>')
			str += add_cmd(str, parsed_instruction);
		while (*str && *str == ' ')
			str++;
	}
	parsed_instruction->cmd_complete
		= create_args(parsed_instruction->cmd_and_its_flags,
			&(parsed_instruction->cmd), data);
}

t_cmd	*split_and_parse_instruction(char *str, t_data *data)
{
	int		i;
	t_cmd	*parsed_instruction;

	i = 0;
	parsed_instruction = malloc(sizeof(t_cmd));
	parsed_instruction->next = 0;
	parsed_instruction->stdins = malloc(sizeof(t_files *));
	parsed_instruction->stdins[0] = 0;
	parsed_instruction->stdouts = malloc(sizeof(t_files *));
	parsed_instruction->stdouts[0] = 0;
	parsed_instruction->stderrs = malloc(sizeof(t_files *));
	parsed_instruction->stderrs[0] = 0;
	parsed_instruction->heredocs = malloc(sizeof(t_files *));
	parsed_instruction->heredocs[0] = 0;
	parsed_instruction->cmd = 0;
	parsed_instruction->cmd_complete = 0;
	parsed_instruction->cmd_and_its_flags = 0;
	parsed_instruction->next = 0;
	parse_instruction(str, parsed_instruction, data);
	return (parsed_instruction);
}

void	parsing_handler(t_data *data, char **str)
{
	char	**instructions;
	int		i;

	i = 0;
	instructions = ft_split_pipes(*str, data);
	if (parse_check(instructions[0]) == 0)
		data->syntax_error = 2;
	while (data->syntax_error != 2 && instructions && instructions[i])
	{
		ft_lstadd_back_cmd(data->cmds,
			split_and_parse_instruction(instructions[i], data));
		data->num_cmds++;
		i++;
	}
	free_double_string(instructions);
}
