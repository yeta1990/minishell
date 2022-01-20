#include "minishell.h"

//equivalence table for add_redirection:
//append 0, type 0 -> file redirected to standard input
//append 1, type 0 -> heredoc
//append 0, type 1 -> standard output redirected to a file
//append 1, type 1 -> standard output redirected to a file in append mode

int	add_redirection(char *raw_file, t_cmd *parsed_instruction, int append, int type, t_data *data)
{
	t_files	*file;
	int		size;

	size = 1;
	file = malloc(sizeof(t_files));
	while (raw_file && *raw_file && *raw_file == ' ')
	{
		size++;
		raw_file++;
	}
	//as bash handles joint stdins (<hola<hola2) as separated
	//instructions, strdup_space has been modified and
	//will be modified as long as we extend parse_instruction
	//functionality
	file->name = ft_strdup_space(raw_file, &size, 0);
	if (ft_strlen(file->name) == 0)
	{
		syntax_error(file->name);
		data->syntax_error = 1;
		data->last_code = 258;
	}
	else if (is_valid_infile(file->name) == 0)
	{
		syntax_error(file->name);
		data->syntax_error = 1;
		data->last_code = 258;
	}
	file->next = 0;
	if (append == 0)
		file->append = 0;
	else
		file->append = 1;
	if (type == 0)
    	ft_lstadd_back_files(parsed_instruction->stdins, file);
	else if (type == 1)
    	ft_lstadd_back_files(parsed_instruction->stdouts, file);
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
		parsed_instruction->cmd_and_its_flags = ft_strdup_space(raw_cmd, &size, 1);
	}
	else 
	{
		aux = ft_strjoin (parsed_instruction->cmd_and_its_flags, " ");
		free(parsed_instruction->cmd_and_its_flags);
		word = ft_strdup_space(raw_cmd, &size, 1);
		parsed_instruction->cmd_and_its_flags = ft_strjoin(aux, word);
		free(word);
		free(aux);
	}
//	printf("returned %i\n", size);
	return (size);
}
