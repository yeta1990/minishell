#include "minishell.h"
#include <string.h>

void	parse_instruction(char *s, t_cmd *parsed_instruction)
{
	char *str;

	str = s;
    while (str && *str)
    {
		if (*str == '<')
        {
			str++;
			if (str && *str && *str == '<')
			{
				str++;
				str += add_heredoc(str, parsed_instruction);
			}
			else if (str && *str && *str != '<')
				str += add_infile(str, parsed_instruction);
        }
		else if (*str == '>')
		{
			str++;
			if (str && *str && *str == '>')
			{
				str++;
				str += add_outfile(str, parsed_instruction, 1);
			}
			else if (str && *str && *str != '>')
				str += add_outfile(str, parsed_instruction, 0);
		}
		else if (*str != '<' && *str != '>')
			str += add_cmd(str, parsed_instruction);
    }
	parsed_instruction->cmd_complete = create_args(parsed_instruction->cmd_and_its_flags, &(parsed_instruction->cmd));
}

t_cmd *split_and_parse_instruction(char *str)
{
    int     i;
    t_cmd   *parsed_instruction;

    i = 0;
//	printf("str: %s\n", str);
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
	parse_instruction(str, parsed_instruction);
	return (parsed_instruction);
}
void	check_leaks(void)
{
	system("leaks minishell");
}

int main(int argc, char **argv)
{
    char    *str;
    t_data   data;
	char	**instructions;
	int		i;

//	atexit(check_leaks);
	i = 0;
	argc += 0;
	while (argv[i])
		i++;

	i = 0;
//		write(1, COLOR_GR "minishell" COLOR_RES, 22);
//		printf(COLOR_GREEN "minishell $ " COLOR_RESET);
//        str = readline(COLOR_GR "minishell $ " COLOR_RES "\2");
	while (1)
	{
		str = readline("minishell $ ");
		if (str && ft_strlen(str) > 0)
 		{
			i = 0;
			data.cmds = malloc(sizeof(t_cmd *));
			data.cmds[0] = 0;
			add_history(str);
			instructions = ft_split_w_quotes(str, '|');
			while (instructions && instructions[i])
			{
				ft_lstadd_back_cmd(data.cmds, split_and_parse_instruction(instructions[i]));
				i++;
			}
			//leaks checker, only for testing purposes
			if (strcmp("exit", data.cmds[0]->cmd_complete[0]) == 0)
			{
				free_double_string(instructions);
				ft_bzero(str, ft_strlen(str));
				free(str);
				free_data(&data);
				exit(0);
			}
		//	print_t_cmd(data.cmds);
			execute_commands(&data);
		//	wait(NULL);
			free_double_string(instructions);
			ft_bzero(str, ft_strlen(str));
			free(str);
			str = 0;
			free_data(&data);
		}
		str = 0;
	}
   	return (0);
}
