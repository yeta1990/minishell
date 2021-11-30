/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2021/11/30 15:10:29 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# define ANSI_COLOR_RED     "\x1b[31m"
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_YELLOW  "\x1b[33m"
# define ANSI_COLOR_BLUE    "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN    "\x1b[36m"
# define ANSI_COLOR_RESET   "\x1b[0m"
typedef struct s_files
{
	char			*name;
	struct s_files	*next;
	int				append;
}	t_files;

//cmd: primer parámetro de exexve
//cmd_complete: segundo parametro de execve
//stdins: nombres de archivo de redirecciones stdin
//stdouts: nombres de archivo de redirecciones stout
//stderrs: nombres de archivo de redirecciones sterr
//heredocs 

typedef struct s_cmd
{
	char			*cmd;
	char			**cmd_complete;
	t_files			**stdins;
	t_files			**stdouts;
	t_files			**stderrs;
	t_files			**heredocs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_history
{
	char				*cmd_line;
	struct s_history	*next;
}	t_history;

typedef struct s_data
{
	int			num_cmds;
	int			num_pipes;
	char		**env;
	t_history	**history;
	t_cmd		**cmds;
}	t_data;

//cmd_arrange.c
int	is_cmd(char *file, char **cmd_ok, char *envp[]);
char	**create_args(char *raw_cmd, char **cmd, char *envp[]);
void	cmd_not_raw(char **args);

// list_utils.c
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void	ft_lstadd_back_files(t_files **lst, t_files *new);
t_cmd	*ft_lstlast_cmd(t_cmd *lst);
t_files	*ft_lstlast_files(t_files *lst);

// lexer_setters.c
int		add_infile(char *raw_infile, t_cmd *raw_cmd);
int		add_outfile(char *raw_infile, t_cmd *raw_cmd, int append);
int		add_heredoc(char *raw_keyword, t_cmd *raw_cmd);
int		add_cmd(char *raw_cmd, t_cmd *parsed_instruction);

//path_operations.c
char	**get_paths(char *envp[]);
char	**path_surgery(char **path_to_cut, int path_emergency);
void	free_paths(char **paths);
int		ft_memcmp(const void *s1, const void *s2, size_t n);

//utils
char	**ft_split(char const *s, char c);
char	*ft_strdup_space(const char *s1, int *size);
char	*ft_strdup(const char *s1);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strtrim_full_str(char const *s1, char const *set);
char	**ft_split_mod(char const *str, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	ft_putstr_fd(char *s, int fd);

#endif
