/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:33:49 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 22:46:59 by albgarci         ###   ########.fr       */
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
