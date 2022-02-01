/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 10:46:09 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/wait.h>

t_data	data;

void	parse_instruction(char *s, t_cmd *parsed_instruction, t_data *data)
{
	char *str;

	str = s;
    while (str && *str && data->syntax_error == 0)
    {
		while (*str && *str == ' ')
			str++;
		if (*str == '<')
        {
			str++;
			if (str && *str && *str == '<')
			{
				str++;
				str += add_redirection(str, parsed_instruction, 1, data);
			}
			else// if (str && *str && *str != '<')
				str += add_redirection(str, parsed_instruction, 0, data);
        }
		else if (*str == '>')
		{
			str++;
			if (str && *str && *str == '>')
			{
				str++;
				str += add_redirection(str, parsed_instruction, 3, data);
			}
			else// if (str && *str && *str != '>')
				str += add_redirection(str, parsed_instruction, 2, data);
		}
		else if (*str != '<' && *str != '>')
			str += add_cmd(str, parsed_instruction);
		while (*str && *str == ' ')
			str++;
    }
	parsed_instruction->cmd_complete = create_args(parsed_instruction->cmd_and_its_flags, &(parsed_instruction->cmd), data);
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

void	check_leaks(void)
{
	system("leaks minishell");
}

void	handler_c(int signo)
{
	signo++;
	data.last_code = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	history_management(t_data *data, char **str)
{
	char	*custom_str;

	custom_str = 0;
	if (data->cmd_by_stdin == 0)
		add_history(*str);
	else
	{
		custom_str = ft_strjoin(*str, ft_lstlast_cmd((*data->cmds))->cmd_and_its_flags);
		if (custom_str)
		{
			add_history(custom_str);
			free(custom_str);
		}
		else
			add_history(*str);
	}
	free(*str);
	*str = 0;
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
		ft_lstadd_back_cmd(data->cmds, split_and_parse_instruction(instructions[i], data));
		data->num_cmds++;
		i++;
	}
	free_double_string(instructions);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	struct sigaction	ctrl_c;

	if (argc == 1)
		help_usage();
	ctrl_c.sa_handler = &handler_c;
	ctrl_c.sa_flags = 0;
	ctrl_c.sa_flags |= SA_SIGINFO;
	signal(SIGQUIT, SIG_IGN);
	data.env = create_env(envp);
	data.exp = create_exp(envp);
	data.last_code = 0;
	while (1)
	{
		sigaction(SIGINT, &ctrl_c, NULL);
		str = readline("minishell $ ");
		if (str && ft_strlen(str) > 0)
 		{
			data.cmds = malloc(sizeof(t_cmd *));
			data.cmds[0] = 0;
			data.num_cmds = 0;
			data.syntax_error = 0;
			data.cmd_by_stdin = 0;
			parsing_handler(&data, &str);
			history_management(&data, &str);
			if (testing_mode(argc, argv, &data))
				;
			else if (data.syntax_error == 0)
			 	data.last_code = execute_commands(&data);
			free_data(&data);
			reset_data(&data);
		}
		else if (!str)
		{
			write(1, "exit\n", 5);
			exit(0);
		}
	}
	return (0);
}
