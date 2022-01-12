/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/12 17:56:40 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char** environ;

int	transform_error_code(char *cmd, int err)
{
	// other errors to take into account
	// in bash, when "< /dev/random cat", ctrl-C, exit code 130
	// in bash, when pressing ctrl-C, exit code 1
	// segfault, 134
	if (err == 13)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 20);
		return (126);
	}
	else if (err == 2)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20); 
		return (127);
	}
	else
	{
		perror("minishell");
		return (1);
	}
}

void	exec_middle(t_cmd *cmd, int fds[2], int fds2[2])
{
	int		child;

	pipe(fds2);
	child = fork();
	if (child == 0)
	{
		if (ft_lstlast_files(*(cmd->stdins)))
			ft_dup_infile(cmd->stdins);
		close(fds2[0]);
		dup2(fds[0], 0);
		close(fds[0]);
		dup2(fds2[1], 1);
		close(fds2[1]);
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
}

void	middle_exec_handler(t_cmd **cmd, int fds[2], int num_cmds)
{
	int		fds2[2];
	int		i;

	i = 1;
	while (i < num_cmds - 1)
	{
		exec_middle(*cmd, fds, fds2);
		*cmd = (*cmd)->next;
		close(fds[0]);
		close(fds2[1]);
		fds[0] = fds2[0];
		fds[1] = fds2[1];
		i++;
	}
}

int	execute_commands(t_data *data)
{
	t_cmd 	*node;
	int		fds[2];
	int		last_status;

	last_status = 0;
	node = *data->cmds;
	if (pipe(fds) < 0)
	{
		perror("minishell");
		exit(1);
	}
	ft_exec_first(node, fds);
	node = node->next;
	middle_exec_handler(&node, fds, data->num_cmds);
	if (data->num_cmds > 1)
		last_status = ft_exec_last(node, fds);
	while (wait(&last_status) != -1)
		 ;
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	return (0);
}

void	ft_exec_first(t_cmd *cmd, int fds[2])
{
	pid_t	child;
	char	*env_path;

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
			ft_dup_infile(cmd->stdins);
		close(fds[0]);
		if (cmd->next)
		{
			dup2(fds[1], 1);
			close(fds[1]);
		}
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
	else
		close(fds[1]);
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
		if (ft_lstlast_files(*(cmd->stdins)))
			ft_dup_infile(cmd->stdins);
		else
			dup2(fds[0], 0);
		close(fds[0]);
		if (ft_lstlast_files(*(cmd->stdouts)))
			ft_dup_output(cmd->stdouts);
		if (cmd->cmd && execve(cmd->cmd, &(cmd->cmd_complete[0]), environ) < 0)
			exit(transform_error_code(cmd->cmd, (int) errno));
		else
			exit(0);
	}
	else
		close(fds[0]);
	waitpid(child, &child_status, WNOHANG);
	return (WEXITSTATUS(child_status));
}
