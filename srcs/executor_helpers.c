/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:33:49 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 22:34:09 by albgarci         ###   ########.fr       */
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
