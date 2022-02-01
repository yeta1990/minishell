/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 09:39:45 by crisfern         ###   ########.fr       */
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

int	execute_commands(t_data *data)
{	
	int		i;
	pid_t	pid[data->num_cmds];
	t_cmd	*cmd;
	int		child_status;
	int		first_sgl;
	int		last_sgl;
	int		code;

	code = 0;
	first_sgl = 0;
	last_sgl = 0;
	cmd = *(data->cmds);
	i = 0;
	create_pipes(data->cmds);
	check_heredocs(data);
	while (cmd)
	{
		if (i == 0)
			exit_builtin(data, cmd);
		child_status = check_outside_builtins(data, cmd);
		if (child_status != -100)
		{
			cmd = cmd->next;
			i++;
			continue ;
		}
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
	i = 0;
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
	if ((WIFSIGNALED(child_status) && (WTERMSIG(child_status) == SIGINT)))
		last_sgl = 1;
	if (!first_sgl && last_sgl)
		return (code);
	else if (first_sgl)
		return (130);
	else if (WIFEXITED(child_status))
		return (WEXITSTATUS(child_status));
	else if (WTERMSIG(child_status) == SIGSEGV)
		return (transform_error_code(0, 11));
	return (child_status);
}
