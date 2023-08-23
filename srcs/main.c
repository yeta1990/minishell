/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/04 12:08:20 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/wait.h>

t_data g_data;

void	handler_c(int signo)
{
	signo++;
	g_data.last_code = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	history_management(t_data *data, char **str)
{
	char	*custom_str;

	custom_str = 0;
	if (data->cmd_by_stdin == 0)
		add_history(*str);
	else
	{
		custom_str = ft_strjoin(*str,
				ft_lstlast_cmd((*data->cmds))->cmd_and_its_flags);
		if (custom_str)
		{
			add_history(custom_str);
			free(custom_str);
		}
		else
			add_history(*str);
	}
	free(*str);
	*str = 0;
}

void	main_controller(t_data *data, char *str)
{
	data->cmds = malloc(sizeof(t_cmd *));
	data->cmds[0] = 0;
	data->num_cmds = 0;
	data->syntax_error = 0;
	data->cmd_by_stdin = 0;
	data->sgl_heredoc = 0;
	parsing_handler(data, &str);
	history_management(data, &str);
	if (data->syntax_error == 0)
		data->last_code = execute_commands(data);
	free_data(data);
	reset_data(data);
}

void	init_env(char **envp)
{
	if (*envp)
	{
		g_data.env = create_env(envp);
		g_data.exp = create_exp(envp);
		inc_shell_level();
	}
	else
		no_env_var();
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	struct sigaction	ctrl_c;

	init_checker(argc, argv);
	ctrl_c.sa_handler = &handler_c;
	ctrl_c.sa_flags = 0;
	ctrl_c.sa_flags |= SA_SIGINFO;
	signal(SIGQUIT, SIG_IGN);
	init_env(envp);
	g_data.last_code = 0;
	while (1)
	{
		sigaction(SIGINT, &ctrl_c, NULL);
		str = readline("minishell $ ");
		if (str && ft_strlen(str) > 0)
			main_controller(&g_data, str);
		else if (!str)
		{
			write(1, "exit\n", 5);
			exit(0);
		}
	}
	return (0);
}
