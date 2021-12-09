
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
			printf("added infile: %s\n", std_aux->name);
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
