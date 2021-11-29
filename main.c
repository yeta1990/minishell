#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

t_cmd    *parse_cmd(char *str, t_cmd *raw_cmd)
{
    t_cmd   cmd;
    t_files *file;

    while (str)
    {
        if (*str++ == '<')
        {
            if (*str == ' ')
                str++;
            file->name = ft_strdup(str);
            ft_lstadd_back_files(raw_cmd->stdins, file);
        }
        str++;
    }
}

void    save_cmd(char *str, t_data *data)
{
    char **split;
    int     i;
    t_cmd   raw_cmd;

    i = 0;
    split = ft_split(str, '|');
    while (split[i])
    {
        ft_lstadd_back_cmd(data->cmds, parse_cmd(split[i], &raw_cmd));
        i++;
    }
    
}

int main(int argc, char **argv, char **envp)
{
    char    *str;
    t_data   data;

    data.env = envp;
    while (1)
    {
        str = readline("minishell");
        save_cmd(str, &data);
        free(str);
    }
}