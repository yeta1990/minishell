/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 12:59:56 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/01 12:32:27 by crisfern         ###   ########.fr       */
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

char	*prepare_file_to_save(int i)
{
	char	*tmp_num;
	char	*path_tmp;
	char	*filename;
	int		fd;

	filename = 0;
	tmp_num = ft_itoa(i);
	path_tmp = ft_strdup("/tmp/minishell");
	filename = ft_strjoin(path_tmp, tmp_num);
	free(tmp_num);
	free(path_tmp);
	fd = open(filename, O_TRUNC, 0644);
	close(fd);
	return (filename);
}

void	run_heredoc_2(t_files **f, int i)
{
	int		fd;
	char	*str;
	char	*filename;
	int		fds[2];
	int		pid;

	filename = prepare_file_to_save(i);
	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(fds[1]);
		close(fds[0]);
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		str = readline("> ");
		while (ft_strlen(str) == 0 || check_eof(str, (*f)->name) == 0)
		{
			if (!str)
				break ;
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
	}
	signal(SIGINT, SIG_IGN);
	close(fds[0]);
	close(fds[1]);
	wait(NULL);
	free((*f)->name);
	(*f)->name = ft_strdup(filename);
	free(filename);
}

void	ft_dup_infile(t_files **stdins)
{
	int		fd;
	t_files	*f;

	f = *stdins;
	while (f)
	{
		fd = open(f->name, O_RDONLY);
		if (fd < 0)
			file_error(f->name, errno);
		if (!(f->next))
		{
			dup2(fd, 0);
			close(fd);
		}
		close(fd);
		if (f->append == 1)
			unlink(f->name);
		f = f->next;
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
