/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 21:38:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/02/04 09:54:34 by crisfern         ###   ########.fr       */
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

void	heredoc_process(t_files **f, char *filename)
{
	int		fd;
	char	*str;

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
}

void	run_heredoc_2(t_files **f, int i, int child_status)
{
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
		heredoc_process(f, filename);
		exit(0);
	}
	signal(SIGINT, SIG_IGN);
	close(fds[0]);
	close(fds[1]);
	wait(&child_status);
	if (WIFSIGNALED(child_status) == 1 && WTERMSIG(child_status) == SIGINT)
		g_data.sgl_heredoc = 1;
	free((*f)->name);
	(*f)->name = ft_strdup(filename);
	free(filename);
}
