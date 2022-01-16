/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 12:59:56 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/16 20:18:45 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_eof(char *str, char *eof)
{
	if (ft_strlen(str) != ft_strlen(eof))
		return (0);
	if (ft_strncmp(str, eof, ft_strlen(str)) == 0)
		return (1);
	return (0);
}

void	run_heredoc(t_files **f)
{
	int		fd;
	char	*str;
	
	fd = open("/tmp/minishell", O_TRUNC, 0644);
	close(fd);
	fd = open("/tmp/minishell", O_WRONLY | O_CREAT | O_APPEND, 0644);
	str = readline("> ");
	while (ft_strlen(str) == 0 || check_eof(str, (*f)->name) == 0)
	{
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
		str = readline("> ");
	}
	if (str)
		free(str);
	str = 0;
	close(fd);
	exit(0);
	free((*f)->name);
	(*f)->name = ft_strdup("/tmp/minishell");
 /*   waitpid(childPid, &returnStatus, 0);
	 if (returnStatus == 0)  // Verify child process terminated without error.
    {
       ft_putstr_fd("The child process terminated normally.", 2);
    }*/
}

void	ft_dup_infile(t_files **stdins)
{
	int		fd;
	t_files	*f;

	f = *stdins;
	while (f)
	{
		if (f->append == 1)
		{
			pid_t childPid;
			childPid = fork();
			if (childPid == 0)
				run_heredoc(&f);
			else
			{
				int returnstatus;
				waitpid(childPid, &returnstatus, 0);

				if (returnstatus == 0)
				{
				fd = open(f->name, O_RDONLY);
				if (fd < 0)
				file_error(f->name, errno);
				if (f->append == 1)
					unlink("/tmp/minishell");
				if (!(f->next))
				{
					dup2(fd, 0);
					close(fd);
				}
				close(fd);
				f = f->next;
				}
			}
		}
		else
		{
		fd = open(f->name, O_RDONLY | O_NONBLOCK);
		if (fd < 0)
			file_error(f->name, errno);
		if (f->append == 1)
			unlink("/tmp/minishell");
		if (!(f->next))
		{
			dup2(fd, 0);
			close(fd);
		}
		close(fd);
		f = f->next;
		}
	}
}

void	ft_dup_output(t_files **stdouts)
{
	int		fdout;
	t_files	*f;

	f = *stdouts;
	while (f)
	{
		if (f->append == 0)
			fdout = open(f->name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		if (f->append == 1)
			fdout = open(f->name, O_CREAT | O_WRONLY | O_APPEND, 0666);
		if (fdout < 0)
			file_error(f->name, errno);
		if (!(f->next))
		{
			close(1);
			dup(fdout);
		}
		close(fdout);
		f = f->next;
	}
}
