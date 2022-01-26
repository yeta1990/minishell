/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/26 18:19:15 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipes(t_cmd **cmds)
{
	t_cmd	*node;

	node = *cmds;
	node->prev_fd = 0;
	while (node->next)
	{
		pipe(node->fd);
		node->next->prev_fd = node->fd;
		node = node->next;
	}
}

void	close_pipes(t_cmd **cmds)
{
	t_cmd	*node;

	node = *cmds;
	while (node->next)
	{
		close(node->fd[0]);
		close(node->fd[1]);
		node->next->prev_fd = node->fd;
		node = node->next;
	}
}

void	check_heredocs(t_data *data)
{
	t_cmd	*cmd;
	t_files	*f;
	int		i;

	i = 0;
	cmd = *(data->cmds);
	while (cmd)
	{
		if (cmd->stdins && *(cmd->stdins))
		{
			f = *(cmd->stdins);
			while (f)
			{
				if (f->append == 1)
				{
					run_heredoc_2(&f, i);
					i++;
				}
				f = f->next;
			}
		}
		cmd = cmd->next;
	}
}

int	execute_commands(t_data *data)
{	
	int		i;
	pid_t	pid;
	t_cmd	*cmd;
	int		child_status;

	cmd = *(data->cmds);
	i = 0;
	create_pipes(data->cmds);
	check_heredocs(data);
	while (cmd)
	{
		if (i == 0)
			exit_builtin(data, cmd);
		if (cmd->cmd && ft_strlen(cmd->cmd_complete[0]) > 1 && check_outside_builtins(data, cmd) == 0)
		{
			pid = fork();
			if (pid == 0)
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
	close_pipes(data->cmds);
	while (wait(&child_status) != -1)
		;
	if (WIFEXITED(child_status))
		return (WEXITSTATUS(child_status));
	else if (WIFSIGNALED(child_status))
		if (WTERMSIG(child_status) == SIGSEGV)
			return (transform_error_code(0, 11));
	return (child_status);
}
