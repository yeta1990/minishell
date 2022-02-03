/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/03 12:41:01 by crisfern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_children(int *child_status, int *code, int *signal)
{
	int		success;
	int		last_pid;
	int		pid;

	pid = 0;
	last_pid = 0;
	while (pid != -1)
	{
		if (WTERMSIG(*child_status) == SIGINT)
			if (last_pid == 0)
				*signal = 1;
		if (pid > last_pid)
		{
			last_pid = pid;
			*code = WEXITSTATUS(*child_status);
			success = WIFEXITED(*child_status);
		}
		pid = wait(child_status);
	}
	return (success);
}

int	builtins_execution(int *i, t_data *data, t_cmd **cmd, int *child_status)
{
	if (*i == 0)
		exit_builtin(data, *cmd);
	if (ft_strcmp("exit", (*cmd)->cmd_complete[0]) != 0)
	{
		*child_status = check_outside_builtins(data, *cmd);
		if (*child_status != -100)
		{
			*cmd = (*cmd)->next;
			(*i)++;
			return (1);
		}
	}
	return (0);
}

void	into_the_fork(t_data *data, t_cmd *cmd, int i)
{
	pipes_and_dups_works(data, cmd, i);
	if (cmd->cmd && check_builtins(data, cmd) == 1)
		exit(data->last_code);
	else if (ft_strlen(cmd->cmd) > 0 || (ft_strlen(cmd->cmd) == 0
			&& !(*cmd->stdins) && !(*cmd->stdouts)
			&& ft_strchr(cmd->cmd_and_its_flags, '$') == 0))
	{
		if (cmd->cmd && execve(cmd->cmd,
				&(cmd->cmd_complete[0]), data->env) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
	}
	else
		exit(0);
}

void	execute_process(t_data *data, int *child_status)
{
	int		i;
	t_cmd	*cmd;

	cmd = *(data->cmds);
	i = 0;
	while (cmd)
	{
		if (builtins_execution(&i, data, &cmd, child_status) == 1)
			continue ;
		else if (cmd->cmd)
		{
			data->pid[i] = fork();
			if (data->pid[i] == 0)
				into_the_fork(data, cmd, i);
			else
				signal(SIGINT, SIG_IGN);
		}
		cmd = cmd->next;
		i++;
	}
}

int	execute_commands(t_data *data)
{	
	int		child_status;
	int		code;
	int		signal;
	int		success;

	signal = 0;
	code = 0;
	create_pipes(data->cmds);
	check_heredocs(data);
	execute_process(data, &child_status);
	close_pipes(data->cmds);
	success = wait_children(&child_status, &code, &signal);
	if (WTERMSIG(child_status) == SIGSEGV)
		return (transform_error_code(0, 11));
	else if (signal == 1)
		return (130);
	else if (success)
		return (code);
	return (child_status);
}
