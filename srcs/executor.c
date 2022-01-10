/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/10 18:09:01 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_commands(t_data *data)
{
	t_cmd 	*node;
	int		fds[2];

	node = *data->cmds;
	if (pipe(fds) < 0)
	{
		perror("minishell");
		exit(1);
	}
	if (data->num_cmds == 1)
	{
		ft_exec_first(node, fds);
		wait(NULL);
	}
	else if (data->num_cmds == 2)
	{
		
	}
	else if (data->num_cmds >= 3)
	{
	
	}
/*	while (node)
	{
		ft_exec_first(node, fds);
		node = node->next;
		wait(NULL);
	}*/
/*	while (i < waits)
	{
		wait(NULL);
		i++;
	}*/
	//	wait(NULL);
	//	node = node->next;

//	dup2(fd_original, 0);
	//	return (ft_exec_last(node, fds));

	return (1);
}

void	ft_exec_first(t_cmd *cmd, int fds[2])
{
//	int		child_status;
	pid_t	child;
	char	*env_path;
//	int		fd;

	env_path = getenv("PATH");
	child = fork();
	if (child == -1)
	{
		perror("minishell");
		exit(errno);
	}
	else if (child == 0)
	{
		if (ft_lstlast_files(*(cmd->stdins)))
			ft_dup_infile(ft_lstlast_files(*(cmd->stdins))->name);
		close(fds[0]);
//		maybe necessary to handle pipes and redirections
		if (cmd->next)
		{
//			printf("hay siguiente\n");
			dup2(fds[1], 1);
			close(fds[1]);
		}
		if (execve(cmd->cmd, &(cmd->cmd_complete[0]), 0) < 0)
		{
			perror("minishell");
			exit(errno);
		}
	}
	else
	{
//		maybe necessary to handle pipes and redirections
//		dup2(fds[1], 1);
		close(fds[1]);
	//	close(fds[0]);
//		this wait must disappear, instead it have to be placed 
//		at the end of all processes. i'm leaving both cases, with and
//		without wnohang just in case
//		waitpid(child, &child_status, 0);
//		waitpid(child, &child_status, WNOHANG);
	}
}

int	ft_exec_last(t_cmd *cmd, int fds[2])
{
	int		child_status;
	pid_t	child;
	char	*env_path;

	env_path = getenv("PATH");
	child = fork();
	if (child == -1)
	{
		perror("minishell");
		exit(errno);
	}
	if (child == 0)
	{
		dup2(fds[0], 0);
		close(fds[0]);
		execve(cmd->cmd, &(cmd->cmd_complete[0]), &env_path);
		perror("minishell");
		exit(errno);
	}
	waitpid(child, &child_status, 0);
	close(fds[0]);
	return (WEXITSTATUS(child_status));
}
