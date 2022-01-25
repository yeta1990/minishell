/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 12:59:56 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 15:58:22 by albgarci         ###   ########.fr       */
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

void	run_heredoc_2(t_files **f, t_cmd *cmd, int i)
{
	int		fd;
	char	*str;
	char	*filename;
	char	*tmp_num;
	char	*path_tmp;

	cmd += 0;
	i += 0;
	tmp_num = ft_itoa(i);
	path_tmp = ft_strdup("/tmp/minishell");
	filename = ft_strjoin(path_tmp, tmp_num);
	signal(SIGINT, SIG_DFL);
	fd = open(filename, O_TRUNC, 0644);
	close(fd);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
	free((*f)->name);
	(*f)->name = ft_strdup(filename);
	str = 0;
	close(fd);
	free(tmp_num);
	free(path_tmp);
	free(filename);
}

void	run_heredoc(t_files **f)
{
	int		fd;
	char	*str;

	signal(SIGINT, SIG_DFL);
	fd = open("/tmp/minishell", O_TRUNC, 0644);
	close(fd);
	fd = open("/tmp/minishell", O_WRONLY | O_CREAT | O_APPEND, 0644);
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
	free((*f)->name);
	(*f)->name = ft_strdup("/tmp/minishell");
	str = 0;
	close(fd);
}

char	*get_cmd_from_user(t_data *data)
{
	char	*input;
	char	*str;
	int		i;
	int		j;
	char	*aux;

	i = 0;
	j = 0;
	input = 0;
	aux = 0;
	signal(SIGINT, SIG_DFL);
	str = readline("> ");
	while (str)
	{
		if (input == 0)
			input = ft_substr(str, 0, ft_strlen(str));
		else
		{
			free(input);
			input = ft_substr(str, 0, ft_strlen(str));
		}
		while (str && str[i])
		{
			if (str[i] == ' ')
				j++;
			i++;
		}
		if (i != j && ft_strlen(str) > 0)
			break ;
		free(str);
		str = 0;
		str = readline("> ");
		i = 0;
		j = 0;
	}
	if (str)
		free(str);
	data->cmd_by_stdin = 1;
	str = 0;
	return (input);
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
