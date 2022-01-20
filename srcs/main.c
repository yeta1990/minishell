/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/20 19:39:08 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/wait.h>
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
				str += add_redirection(str, parsed_instruction, 1, 0, data);
			}
			else// if (str && *str && *str != '<')
				str += add_redirection(str, parsed_instruction, 0, 0, data);
        }
		else if (*str == '>')
		{
			str++;
			if (str && *str && *str == '>')
			{
				str++;
				str += add_redirection(str, parsed_instruction, 1, 1, data);
			}
			else// if (str && *str && *str != '>')
				str += add_redirection(str, parsed_instruction, 0, 1, data);
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

void	handler_c(int a)
{
	a = 0;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	t_data				data;
	char				**instructions;
	int					i;
	struct sigaction	ctrl_c;
	
	ctrl_c.sa_handler = &handler_c;
	ctrl_c.sa_flags = 0;
	signal(SIGQUIT, SIG_IGN);
	if (argc == 1)
		help_usage();
	if (argc == 2 && argv[1][0] == 51)
		atexit(check_leaks);
	i = 0;
	argc += 0;
	data.env = create_env(envp);
	data.exp = create_exp(envp);
	while (argv[i])
		i++;
	i = 0;
	data.last_code = 0;
	while (1)
	{
		sigaction(SIGINT, &ctrl_c, NULL);
		str = readline("minishell $ ");
		if (str && ft_strlen(str) > 0)
 		{
			i = 0;
			data.cmds = malloc(sizeof(t_cmd *));
			data.cmds[0] = 0;
			data.num_cmds = 0;
			data.syntax_error = 0;
			add_history(str);
			instructions = ft_split_w_quotes(str, '|');
			while (instructions && instructions[i])
			{
				ft_lstadd_back_cmd(data.cmds, split_and_parse_instruction(instructions[i], &data));
				data.num_cmds++;
				i++;
			}
			free_double_string(instructions);
			free(str);
			str = 0;
			if (argc == 2 && argv[1][0] == 49)
			{
				printf("test mode 1\n");
				print_t_cmd(data.cmds);
				data.last_code = execute_commands(&data);
			}
			else if (argc == 2 && argv[1][0] == 48)
			{
				printf("test mode 0\n");
				data.last_code = execute_commands(&data);
				printf("exit status code: %i\n", data.last_code);
			}
			else if (argc == 2 && argv[1][0] == 50)
			{
				printf("test mode 2\n");
				print_t_cmd(data.cmds);
				data.last_code = execute_commands(&data);
				printf("exit status code: %i\n", data.last_code);
			}
			else if (data.syntax_error == 0)
				data.last_code = execute_commands(&data);
			free_data(&data);
			reset_data(&data);
		}
		else
			exit(0);
	}
	return (0);
}
