#include "minishell.h"

void	parse_instruction(char *str, t_cmd *parsed_instruction)
{
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
	parsed_instruction->cmd_complete = create_args(parsed_instruction->cmd_and_its_flags, &(parsed_instruction->cmd), *(parsed_instruction->env));
}

t_cmd *split_and_parse_instruction(char *str, t_data *data)
{
//    char **split;
    int     i;
    t_cmd   *parsed_instruction;

    i = 0;
//	split = ft_split_w_quotes(str, '|');
//	while (split && *split)
//		i++;
	printf("%s", str);	
	parsed_instruction = malloc(sizeof(t_cmd));
	parsed_instruction->next = 0;
	parsed_instruction->stdins = malloc(sizeof(t_files *) * 2);
	parsed_instruction->stdins[0] = 0;
	parsed_instruction->stdins[1] = 0;
	parsed_instruction->stdouts = malloc(sizeof(t_files *) * 2);
	parsed_instruction->stdouts[0] = 0;
	parsed_instruction->stdouts[1] = 0;
	parsed_instruction->stderrs = malloc(sizeof(t_files *) * 2);
	parsed_instruction->stderrs[0] = 0;
	parsed_instruction->stderrs[1] = 0;
	parsed_instruction->heredocs = malloc(sizeof(t_files *) * 2);
	parsed_instruction->heredocs[0] = 0;
	parsed_instruction->heredocs[1] = 0;
	parsed_instruction->cmd = 0;
	parsed_instruction->cmd_complete = 0;
	parsed_instruction->cmd_and_its_flags = 0;
	parsed_instruction->next = 0;
	parsed_instruction->env = &(data->env);
 //   while (split && split[i])
   // {
		parse_instruction(str, parsed_instruction);
		return (parsed_instruction);
	
    //    ft_lstadd_back_cmd(data->cmds, parsed_instruction);
//        i++;
  //  }
}

int main(int argc, char **argv, char **envp)
{
    char    *str;
    t_data   data;
	int		i;
	char	**instructions;

	i = 0;
	argc += 0;
	while (argv[i])
		i++;
	data.env = envp;
	data.cmds = malloc(sizeof(t_cmd *) * 2);
	data.cmds[0] = 0;
	data.cmds[1] = 0;
    while (1)
    {
        str = readline(ANSI_COLOR_GREEN "minishell $ " ANSI_COLOR_RESET);
		if (str)
 		{
			i = 0;
			instructions = ft_split_w_quotes(str, '|');
			while (instructions && *instructions && i < 2)
			{
				//ft_lstadd_back_cmd(data.cmds, split_and_parse_instruction(*instructions, &data));
				printf("instr: %s\n", instructions[i]);
			//	instructions++;
				i++;
			}
			free(str);

		/*	if (ft_strlen(str) > 0)
			{
				print_t_cmd(data.cmds[0]);
				//we should free data.cmds after each iteration to prevent minishell
				//from leaks and ease the work
				data.cmds[0] = 0;
			}*/
		}
		if (argc == 1000000)
			return (0);
	}
   	return (0);
}
