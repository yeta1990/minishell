/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:31:46 by albgarci          #+#    #+#             */
/*   Updated: 2021/12/13 12:21:44 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_commands(t_data *data)
{
	t_cmd 	*node;
//	t_files	*stdouts;
	int		fds[2];
	int		fd;
	int		fd_original;

	fd_original = dup(0);

	node = *data->cmds;
	if (ft_lstlast_files(*(node->stdins)))
	{
	
		fd = open(ft_lstlast_files(*(node->stdins))->name, O_RDONLY | O_NONBLOCK);
		dup2(fd, 0);
		close(fd);
		//ft_dup_infile(ft_lstlast_files(*(node->stdins))->name);
		
		if (pipe(fds) < 0)
		{
			perror("minishell");
			exit(1);
		}

	//	close(0);
	//	dup2(fd, 0);
//		close(0);

	}

//	printf("%s,", ft_lstlast_files(*(node->stdins))->name);

//	while(node)
//	{
		//	node = node->next;
	/*	stdouts = *node->stdouts;
		while (stdouts)
		{
			ft_dup_output(stdouts->name);
			stdouts = stdouts->next;
		}
*/
	ft_exec_first(node, fds);
	dup2(fd_original, 0);
	//	node = node->next;
	//	return (ft_exec_last(node, fds));

//	}
	return (1);
}

void	ft_exec_first(t_cmd *cmd, int fds[2])
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
	else if (child == 0)
	{
		write(1, "hola", 4);
		close(fds[0]);
//		dup2(fds[1], 1);
//		close(fds[1]);
	//	exit(0);
		
		if (execve(cmd->cmd, &(cmd->cmd_complete[0]), 0) < 0)
		{
			perror("minishell");
			exit(errno);
		}

	}
	else
	{
//		dup2(fds[1], 1);
		close(fds[1]);
		waitpid(child, &child_status, 0);
		//waitpid(child, &child_status, WNOHANG);

	//	close(fds[0]);

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
