/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 22:34:32 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipes_and_dups_works(t_data *data, t_cmd *cmd, int i)
{
	if (i < data->num_cmds - 1)
	{
		dup2(cmd->fd[1], 1);
		close(cmd->fd[1]);
	}
	if (i != 0)
	{
		dup2((cmd->prev_fd)[0], 0);
		close((cmd->prev_fd)[0]);
	}
	ft_dup_infile(cmd->stdins);
	ft_dup_output(cmd->stdouts);
	close_pipes(data->cmds);
}

int	pipeline_final(int *child_status, int *code, int *first_sgl)
{
	int		last_sgl;
	int		i;

	i = 0;
	last_sgl = 0;
	if ((WIFSIGNALED(*child_status) && (WTERMSIG(*child_status) == SIGINT)))
		last_sgl = 1;
	if (!(*first_sgl) && last_sgl)
		return (*code);
	else if (*first_sgl)
		return (130);
	else if (WIFEXITED(*child_status))
		return (WEXITSTATUS(*child_status));
	else if (WTERMSIG(*child_status) == SIGSEGV)
		return (transform_error_code(0, 11));
	return (*child_status);
}

int		builtins_execution(int *i, t_data *data, t_cmd *cmd, int *child_status)
{
	if (*i == 0)
		exit_builtin(data, cmd);
	*child_status = check_outside_builtins(data, cmd);
	if (*child_status != -100)
	{
		cmd = cmd->next;
		(*i)++;
		return (1);
	}
	return (0);
}

void	execute_process(t_data *data, t_cmd *cmd, pid_t *pid, int *child_status)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (builtins_execution(&i, data, cmd, child_status))
			continue ;
		else if (cmd->cmd && ft_strlen(cmd->cmd_complete[0]) > 0)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				pipes_and_dups_works(data, cmd, i);
				if (cmd->cmd && check_builtins(data, cmd) == 1)
					exit(data->last_code);
				else if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), data->env) < 0)
					exit(transform_error_code(cmd->cmd, (int) errno));
				else
					exit(0);
			}
			else
				signal(SIGINT, SIG_IGN);
		}
		cmd = cmd->next;
		i++;
	}
}


int	execute_commands(t_data *data)
{	
	int		i;
	pid_t	*pid;
	t_cmd	*cmd;
	int		child_status;
	int		first_sgl;
	int		last_sgl;
	int		code;

	pid = malloc(sizeof(pid_t) * data->num_cmds);
	code = 0;
	first_sgl = 0;
	last_sgl = 0;
	cmd = *(data->cmds);
	i = 0;
	create_pipes(data->cmds);
	check_heredocs(data);
	execute_process(data, cmd, pid, &child_status);
	close_pipes(data->cmds);
	while (wait(&child_status) != -1)
	{
		if (i == 0)
		{
			if (WIFEXITED(child_status))
				code = WEXITSTATUS(child_status);
			else if (WIFSIGNALED(child_status) && (WTERMSIG(child_status) == SIGINT))
				first_sgl = 1;
		}
		i++;
	}
	free(pid);
	return (pipeline_final(&child_status, &code, &first_sgl));
}
