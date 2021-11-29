#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	add_infile(char *raw_infile, t_cmd *raw_cmd)
{
	t_files file;

	file.name = ft_strdup_space(raw_infile);
	file.next = 0;
    ft_lstadd_back_files(raw_cmd->stdins, &file);
}

void	parse_cmd(char *str, t_cmd *raw_cmd)
{
    while (str && *str)
    {
        if (*str == '<')
        {
			str++;
            if (str && *str == ' ')
                str++;
			add_infile(str, raw_cmd);
        }
        str++;
    }
}

void    save_cmd(char *str, t_data *data)
{
    char **split;
    int     i;
    t_cmd   *raw_cmd;

    i = 0;
	split = ft_split(str, '|');
	raw_cmd = malloc(sizeof(t_cmd));
	raw_cmd->next = 0;
	raw_cmd->stdins = malloc(sizeof(t_files *) * 2);
	raw_cmd->stdins[0] = 0;
	raw_cmd->stdins[1] = 0;
	raw_cmd->stdouts = malloc(sizeof(t_files *) * 2);
	raw_cmd->stdouts[0] = 0;
	raw_cmd->stdouts[1] = 0;
	raw_cmd->stderrs = malloc(sizeof(t_files *) * 2);
	raw_cmd->stderrs[0] = 0;
	raw_cmd->stderrs[1] = 0;
    while (split && split[i])
    {
		parse_cmd(split[i], raw_cmd);
        ft_lstadd_back_cmd(data->cmds, raw_cmd);
        i++;
    }
}

void	print_data(t_cmd **cmds)
{
	int	i;
	int	j;

	i = 0;
	j = 0;	
	while(cmds && cmds[i])
	{
		while (cmds[i]->stdins[0])
		{
			printf("%s\n", cmds[i]->stdins[0]->name);
			cmds[i]->stdins[0] = cmds[i]->stdins[0]->next;
		}
		i++;
	}
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
        str = readline("minishell");
        save_cmd(str, &data);
        free(str);
		print_data(data.cmds);
    	return (0);
	}

    	return (0);
}
