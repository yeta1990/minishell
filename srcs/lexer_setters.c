#include "minishell.h"

void	add_infile(char *raw_infile, t_cmd *raw_cmd)
{
	t_files *file;

	file = malloc(sizeof(t_files));
	//as bash handles joint stdins (<hola<hola2) as separated
	//instructions, strdup_space has been modified and
	//will be modified as long as we extend parse_instruction
	//functionality
	file->name = ft_strdup_space(raw_infile);
	file->next = 0;
    ft_lstadd_back_files(raw_cmd->stdins, file);
}

void	add_outfile(char *raw_infile, t_cmd *raw_cmd, int append)
{
	t_files *file;

	file = malloc(sizeof(t_files));
	file->name = ft_strdup_space(raw_infile);
	file->next = 0;
	if (append == 0)
		file->append = 0;
	else
		file->append = 1;
	ft_lstadd_back_files(raw_cmd->stdouts, file);
}

void	add_heredoc(char *raw_keyword, t_cmd *raw_cmd)
{
	t_files *keyword;

	keyword = malloc(sizeof(t_files));
	keyword->name = ft_strdup_space(raw_keyword);
	keyword->next = 0;
	ft_lstadd_back_files(raw_cmd->heredocs, keyword);
}
