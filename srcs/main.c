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

void    split_and_parse_instruction(char *str, t_data *data)
{
    char **split;
    int     i;
    t_cmd   *parsed_instruction;

    i = 0;
	split = ft_split(str, '|');
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
    while (split && split[i])
    {
		parse_instruction(split[i], parsed_instruction);
        ft_lstadd_back_cmd(data->cmds, parsed_instruction);
        i++;
    }
}

void	print_data(t_cmd *cmd)
{
	int	i;
	int	j;
	t_files *std_aux;
	char	**cmd_complete;

	cmd_complete = 0;

	i = 0;
	j = 0;
	std_aux = *cmd->stdins;
	printf("\n###### lexer's results ######\n");
	if (std_aux)
		printf("---stdins---\n");
	while (std_aux)
	{
		printf("added infile: %s\n", std_aux->name);
		std_aux = std_aux->next;
		i++;
	}
	std_aux = 0;
	std_aux = *cmd->stdouts;
	if (std_aux)
		printf("---stdouts---\n");
	while (std_aux)
	{
		printf("added outfile %s, append %i\n", std_aux->name, std_aux->append);
		std_aux = std_aux->next;
		i++;
	}
	std_aux = 0;
	std_aux = *cmd->stderrs;
	if (std_aux)
		printf("---stderrs---\n");
	while (std_aux)
	{
		printf("added stderr %s\n", std_aux->name);
		std_aux = std_aux->next;
		i++;
	}
	std_aux = 0;
	std_aux = *cmd->heredocs;
	if (std_aux)
		printf("---heredoc---\n");
	while (std_aux)
	{
		printf("added heredoc with keyword %s\n", std_aux->name);
		std_aux = std_aux->next;
		i++;
	}
	if (cmd->cmd != 0)
		printf("cmd: %s\n", cmd->cmd);
	if (cmd->cmd_complete)
	{
		printf("cmd_complete: ");
		cmd_complete = cmd->cmd_complete;
		while(cmd_complete && *cmd_complete)
		{
			printf("%s, ", *cmd_complete);
			cmd_complete++;
		}
	}
	if (cmd->cmd_and_its_flags)
		printf("\ncmd and its flags (useless for execve): %s", cmd->cmd_and_its_flags);
	printf("\n");
}

int main(int argc, char **argv, char **envp)
{
    char    *str;
    t_data   data;
	int		i;

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
			split_and_parse_instruction(str, &data);
        	free(str);

			if (ft_strlen(str) > 0)
			{
				print_data(data.cmds[0]);
				//we should free data.cmds after each iteration to prevent minishell
				//from leaks and ease the work
				data.cmds[0] = 0;
			}
		}
		if (argc == 1000000)
			return (0);
	}

    	return (0);
}
