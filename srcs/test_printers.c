
#include "minishell.h"

void	print_t_cmd(t_cmd *cmd)
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
