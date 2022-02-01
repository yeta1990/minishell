
#include "minishell.h"

void	print_t_cmd(t_cmd **cmds)
{
	int	i;
	int	j;
	t_files *std_aux;
	char	*cmd_complete;
	t_cmd	*cmd_aux;


	cmd_aux = *cmds;

	while (cmd_aux)
	{	
		cmd_complete = 0;
	
		i = 0;
		j = 0;
		std_aux = *cmd_aux->stdins;
		printf("\n###### lexer's results ######\n");
		if (std_aux)
			printf("---stdins---\n");
		while (std_aux)
		{
			if (std_aux->append == 0)
				printf("added infile: %s\n", std_aux->name);
			if (std_aux->append == 1)
				printf("added heredoc: %s\n", std_aux->name);
			std_aux = std_aux->next;
			i++;
		}
		std_aux = 0;
		std_aux = *cmd_aux->stdouts;
		if (std_aux)
			printf("---stdouts---\n");
		while (std_aux)
		{
			printf("added outfile %s, append %i\n", std_aux->name, std_aux->append);
			std_aux = std_aux->next;
			i++;
		}
		std_aux = 0;
		std_aux = *cmd_aux->stderrs;
		if (std_aux)
			printf("---stderrs---\n");
		while (std_aux)
		{
			printf("added stderr %s\n", std_aux->name);
			std_aux = std_aux->next;
			i++;
		}
		std_aux = 0;
		std_aux = *cmd_aux->heredocs;
		if (std_aux)
			printf("---heredoc---\n");
		while (std_aux)
		{
			printf("added heredoc with keyword %s\n", std_aux->name);
			std_aux = std_aux->next;
			i++;
		}
		if (cmd_aux->cmd != 0)
			printf("cmd: %s\n", cmd_aux->cmd);
		if (cmd_aux->cmd_complete)
		{
			printf("cmd_complete: ");

			i = 0;
			while (cmd_aux->cmd_complete[i])
			{
				printf("%s, ", cmd_aux->cmd_complete[i]);
				i++;
			}
		}
		if (cmd_aux->cmd_and_its_flags)
			printf("\ncmd and its flags (useless for execve): %s", cmd_aux->cmd_and_its_flags);
		printf("\n#########################\n");
		printf("\n");

		cmd_aux = cmd_aux->next;
	}
}

void	help_usage(void)
{
	printf("#############  minishell ###############\n");
	printf("#                                      #\n");
	printf("# available test modes:                #\n");
	printf("# - last exit code: ./minishell 0      #\n");
	printf("# - lexer result ./minishell 1         #\n");
	printf("# - last exit + lexer: ./minishell 2   #\n");
	printf("# - leaks: ./minishell 3               #\n");
	printf("#                                      #\n");
	printf("########################################\n\n");
}

void	check_leaks(void)
{
	system("leaks minishell");
}

int	testing_mode(int argc, char **argv, t_data *data)
{
	if (argc == 2 && argv[1][0] == 49)
	{
		printf("test mode 1\n");
		print_t_cmd(data->cmds);
		data->last_code = execute_commands(data);
		return (1);
	}
	else if (argc == 2 && argv[1][0] == 48)
	{
		printf("test mode 0\n");
		data->last_code = execute_commands(data);
		printf("exit status code: %i\n", data->last_code);
		return (1);
	}
	else if (argc == 2 && argv[1][0] == 50)
	{
		printf("test mode 2\n");
		print_t_cmd(data->cmds);
		data->last_code = execute_commands(data);
		printf("exit status code: %i\n", data->last_code);
		return (1);
	}
	else if (argc == 2 && argv[1][0] == 51)
		atexit(check_leaks);
	return (0);
}
