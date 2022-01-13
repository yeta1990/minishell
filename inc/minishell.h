/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/13 13:11:07 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

typedef struct s_files
{
	char			*name;
	struct s_files	*next;
	int				append;
}	t_files;

//	cmd: primer parámetro de exexve
//	cmd_complete: segundo parametro de execve
//	cmd_and_its_flags: el lexer une todas las palabras sospechosas 
//de ser un comando y sus flags en una cadena de carácteres separadas 
//por espacios. luego esta se utiliza para crear "cmd" y "cmd complete",
//luego ya es inútil.
//	stdins: nombres de archivo de redirecciones stdin
//	stdouts: nombres de archivo de redirecciones stout
//	stderrs: nombres de archivo de redirecciones sterr
//	heredocs

typedef struct s_cmd
{
	char			*cmd;
	char			**cmd_complete;
	char			*cmd_and_its_flags;
	t_files			**stdins;
	t_files			**stdouts;
	t_files			**stderrs;
	t_files			**heredocs;
	struct s_cmd	*next;
}	t_cmd;


typedef struct s_data
{
	int			num_cmds;
	int			num_pipes;
	int			last_code;
	t_cmd		**cmds;
}	t_data;

//cmd_arrange.c
int	is_cmd(char *file, char **cmd_ok);
char	**create_args(char *raw_cmd, char **cmd);
void	cmd_not_raw(char **args);

// list_utils.c
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void	ft_lstadd_back_files(t_files **lst, t_files *new);
t_cmd	*ft_lstlast_cmd(t_cmd *lst);
t_files	*ft_lstlast_files(t_files *lst);
t_files	*ft_lstnew(void *content);

// list_utils_2.c
char	**from_list_to_double_char(t_files **full_strings);

// lexer_setters.c
int		add_redirection(char *raw_file, t_cmd *parsed_instruction, int append, int type);
int		add_cmd(char *raw_cmd, t_cmd *parsed_instruction);

//path_operations.c
char	**get_paths(char *path);
char	**path_surgery(char **path_to_cut, int path_emergency);
void	free_paths(char **paths);
int		ft_memcmp(const void *s1, const void *s2, size_t n);

// get_char_pos.c
int		has_closed_quotes(char *str);
int		get_char_pos(char *str, char c);
char	**split_quote_sensitive(char *str);

//split_quotes.c
int		get_char_pos_final_quotes(char q, char *str);

//utils
char	**ft_split(char const *s, char c);
char	*ft_strdup_space(const char *s1, int *size);
int		std_space_get_cut_position(char *s1);
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
char	**ft_split_w_quotes(char const *str, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

//freezers
void	free_double_string(char **str);
void	free_data(t_data *data);
void	free_files(t_files *first);
void	reset_data(t_data *data);

//test_printers.c
void	print_t_cmd(t_cmd **cmds);
void	help_usage(void);

//executor.c
int		execute_commands(t_data *data);
void	ft_exec_first(t_cmd *data, int fds[2]);
int		ft_exec_last(t_cmd *data, int fds[2]);

//ft_files.c
void	ft_dup_infile(t_files **stdins);
void	ft_dup_output(t_files **stdouts);

char	*ft_strtrim(char const *s1, char const *set);

//error_handlers.c
int	transform_error_code(char *cmd, int err);
void	file_error(char *filename, int errn);
void	std_error(int errn);

#endif
