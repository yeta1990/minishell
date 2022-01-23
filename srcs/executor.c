/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/23 23:39:55 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

void	exec_middle(t_data *data, t_cmd *cmd, int fds[2], int fds2[2])
{
	int		child;
	
	if (pipe(fds2) < 0)
		std_error(errno);
	child = fork();
	if (child == 0)
	{
		if (ft_lstlast_files(*(cmd->stdins))->append == 1)
		{
			close(fds[0]);
			ft_dup_infile(cmd->stdins);
		}
		close(fds2[0]);
		dup2(fds[0], 0);
		if (ft_lstlast_files(*(cmd->stdins))->append == 0)
			ft_dup_infile(cmd->stdins);
		close(fds[0]);
		dup2(fds2[1], 1);
		close(fds2[1]);
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && check_builtins(data, cmd) == 1)
			exit(0);
		else if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
	else
		signal(SIGINT, SIG_IGN);
}

void	middle_exec_handler(t_data *data, t_cmd **cmd, int fds[2])
{
	int		fds2[2];
	int		i;
	int		last_status;

	i = 1;
	last_status = 0;
	while (i < data->num_cmds - 1)
	{
		exec_middle(data, *cmd, fds, fds2);
		close(fds[0]);
		close(fds[1]);
		close(fds2[0]);
		close(fds2[1]);
		wait(&last_status);
		*cmd = (*cmd)->next;
		fds[0] = fds2[0];
		fds[1] = fds2[1];
		i++;
	}
}

int	execute_commands(t_data *data)
{
	t_cmd	*node;
	int		fds[2];
	int		last_status;

	last_status = 0;
	node = *data->cmds;
	if (pipe(fds) < 0)
		std_error(errno);
	ft_exec_first(data, node, fds);
	wait(&last_status);
	node = node->next;
	middle_exec_handler(data, &node, fds);
	while (wait(&last_status) != -1)
		;
	if (data->num_cmds > 1)
		last_status = ft_exec_last(data, node, fds);
	wait(&last_status);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		if (WTERMSIG(last_status) == SIGSEGV)
			return (transform_error_code(0, 11));
	return (0);
}

void	ft_exec_first(t_data *data, t_cmd *cmd, int fds[2])
{
	pid_t	child;

	exit_builtin(data, cmd);
	child = fork();
	if (child == -1)
		std_error(errno);
	else if (child == 0)
	{
		if (ft_lstlast_files(*(cmd->stdins)))
			ft_dup_infile(cmd->stdins);
		close(fds[0]);
		if (cmd->next)
		{
			dup2(fds[1], 1);
			close(fds[1]);
		}
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && check_builtins(data, cmd) == 1)
			exit(data->last_code);
		else if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
	//	if (!(cmd->next))
			close(fds[0]);
		close(fds[1]);
	}
}

int	ft_exec_last(t_data *data, t_cmd *cmd, int fds[2])
{
	int		child_status;
	pid_t	child;

	child = fork();
	if (child == -1)
		std_error(errno);
	if (child == 0)
	{

		if (ft_lstlast_files(*(cmd->stdins)))
			ft_dup_infile(cmd->stdins);
	//	else
		dup2(fds[0], 0);
		close(fds[0]);
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && check_builtins(data, cmd) == 1)
			exit(data->last_code);
		else if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(fds[0]);
	}
//	waitpid(child, &child_status, WNOHANG);
	return (WEXITSTATUS(child_status));
}
