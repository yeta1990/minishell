#include "minishell.h"

int	add_infile(char *raw_infile, t_cmd *parsed_instruction)
{
	t_files	*file;
	int		size;

	size = 1;
	file = malloc(sizeof(t_files));
	while (raw_infile && *raw_infile && *raw_infile == ' ')
	{
		size++;
		raw_infile++;
	}
	//as bash handles joint stdins (<hola<hola2) as separated
	//instructions, strdup_space has been modified and
	//will be modified as long as we extend parse_instruction
	//functionality
	file->name = ft_strdup_space(raw_infile, &size);
	file->next = 0;
    ft_lstadd_back_files(parsed_instruction->stdins, file);
	return (size);
}

int	add_outfile(char *raw_outfile, t_cmd *parsed_instruction, int append)
{
	t_files	*file;
	int		size;

	size = 1;
	file = malloc(sizeof(t_files));
	while (raw_outfile && *raw_outfile && *raw_outfile == ' ')
	{
		size++;
		raw_outfile++;
	}
	file->name = ft_strdup_space(raw_outfile, &size);
	file->next = 0;
	if (append == 0)
		file->append = 0;
	else
		file->append = 1;
	ft_lstadd_back_files(parsed_instruction->stdouts, file);
	return (size);
}

int	add_heredoc(char *raw_keyword, t_cmd *parsed_instruction)
{
	t_files	*keyword;
	int		size;

	size = 1;
	keyword = malloc(sizeof(t_files));
	while (raw_keyword && *raw_keyword && *raw_keyword == ' ')
	{
		raw_keyword++;
		size++;
	}
	keyword->name = ft_strdup_space(raw_keyword, &size);
	keyword->next = 0;
	ft_lstadd_back_files(parsed_instruction->heredocs, keyword);
//	printf("size heredoc %i\n", size);
	return (size);
}

int	add_cmd(char *raw_cmd, t_cmd *parsed_instruction)
{
	int		size;
	char	*aux;
	char	*word;

	aux = 0;
	size = 0;
//	printf("raw_cmd in add_cmd: %s\n", raw_cmd);
	if (parsed_instruction->cmd_and_its_flags == 0)
	{
		parsed_instruction->cmd_and_its_flags = ft_strdup_space(raw_cmd, &size);
	}
	else 
	{
		aux = ft_strjoin (parsed_instruction->cmd_and_its_flags, " ");
		free(parsed_instruction->cmd_and_its_flags);
		word = ft_strdup_space(raw_cmd, &size);
		parsed_instruction->cmd_and_its_flags = ft_strjoin(aux, word);
		free(word);
		free(aux);
	}
//	printf("returned %i\n", size);
	return (size);
}
