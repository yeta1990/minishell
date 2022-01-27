/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 13:32:33 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/27 18:54:14 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "readline/readline.h"
# include "readline/history.h"
# include <sys/param.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>

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
	int				fd[2];
	int				*prev_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	int					num_cmds;
	int					num_pipes;
	int					last_code;
	t_cmd				**cmds;
	char				**env;
	char				**exp;
	int					syntax_error;
	int					cmd_by_stdin;
}	t_data;

typedef struct s_pipe_sep_vars
{
	int		nwords;
	int		f_dquote;
	int		f_quote;
	int		i;
	int		last;
	t_files	**separated_pipes;
}	t_pipe_sep_vars;

typedef struct s_split_cmds_vars
{
	char	*aux;
	t_files	**full_strings;
	int		flag;
	char	*result;
	char	*r;
	char	*trimmed;
	char	*subs;
	char	*expanded;
	int		forward;
}	t_split_cmds_vars;

typedef struct s_expansor_vars
{
	char	*a;
	char	*exp;
	char	*aux_exp;
	char	*aux_exp2;
	char	*aux_exp3;
	char	*tail;
	int		i;
	int		j;
	int		z;		
}	t_expansor_vars;

void	handler_c(int a);

//cmd_arrange.c
int		is_cmd(char *file, char **cmd_ok, t_data *data);
char	**create_args(char *raw_cmd, char **cmd, t_data *data);
void	cmd_not_raw(char **args);
char	*expansor(char **arg, int type, t_data *data);

// list_utils.c
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void	ft_lstadd_back_files(t_files **lst, t_files *new);
t_cmd	*ft_lstlast_cmd(t_cmd *lst);
t_files	*ft_lstlast_files(t_files *lst);
t_files	*ft_lstnew(void *content);

// list_utils_2.c
char	**from_list_to_double_char(t_files **full_strings);

// lexer_setters.c
int		add_redirection(char *raw_file, t_cmd *cmd, int type, t_data *data);
int		add_cmd(char *raw_cmd, t_cmd *parsed_instruction);

//path_operations.c
char	**get_paths(char *path);
char	**path_surgery(char **path_to_cut, int path_emergency);
void	free_paths(char **paths);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*our_getenv(t_data *data, char *to_find);

// get_char_pos.c
int		has_closed_quotes(char *str);
int		get_char_pos(char *str, char c);
char	**split_quote_sensitive(char *str, t_data *data);

//split_quotes.c
int		get_char_pos_final_quotes(char *str);
int		search_after_pipe(int len, int j, char *aux, t_data *data);
char	*search_next_pipe(int *len, char *aux, t_data *data);

//split_quotes_2.c
void			set_quotes_flags(char c, int *f_dquote, int *f_quote);
int				get_nwords(char *str, char c);
int				check_pipes_syntax_error(char *str, t_data *data);
void			add_pipe(t_files **sep_pipes, char *s, t_pipe_sep_vars *w, t_data *data);
t_pipe_sep_vars	*initialise_save_words_vars(void);

//utils
char	**ft_split(char const *s, char c);
char	*ft_strdup_space(const char *s1, int *size, int cmd);
int		std_space_get_cut_position(char *s1);
int		std_space_get_cut_space(char *s1);
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
char	**ft_split_pipes(char const *str, t_data *data);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_itoa(int n);
int		ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isdigit(int c);

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
void	ft_exec_first(t_data *data, t_cmd *cmd, int fds[2]);
int		ft_exec_last(t_data *data, t_cmd *cmd, int fds[2]);

//ft_files.c
void	ft_dup_infile(t_files **stdins);
void	ft_dup_output(t_files **stdouts);
char	*ft_strtrim(char const *s1, char const *set);
char	*get_cmd_from_user(t_data *data);
void	run_heredoc_2(t_files **f, int i);

//error_handlers.c
int		transform_error_code(char *cmd, int err);
void	file_error(char *filename, int errn);
void	std_error(int errn);
int		isalnum_string(char *str);
int		is_valid_infile(char *str);
void	syntax_error(char *wrong_portion, t_data *data);
int		parse_check(char *str);
int		cd_error(char *filename, int errn, t_data *data);
int		export_error(char *filename, t_data *data);

//envp.c
int		get_env_size(char **envp);
char	**create_env(char **envp);
char	**create_exp(char **envp);
void	free_env(char **env);
char	*export_join(char *str);

//builtins1.c
void	pwd_builtin(void);
int		cd_bultin(t_data *data, t_cmd *cmd);
void	env_builtin(t_data *data);
int		exit_builtin(t_data *data, t_cmd *cmd);
int		check_builtins(t_data *data, t_cmd *cmd);
int		check_outside_builtins(t_data *data, t_cmd *cmd);

//builtins2.c
int		echo_flag(char **cmd_complete);
void	echo_builtin(t_cmd *cmd);
void	update_env(t_data *data, int index_exp, int i);
int		export_builtin(t_data *data, t_cmd *cmd);
int		unset_builtin(t_data *data, t_cmd *cmd);

//builtins3.c
int		search_word_del(char **arr, char *str);
int		search_word(char **arr, char *str);
char	**add_entry(char **old_arr, char *new_str);
char	**del_entry(char **old_arr, int index);

//quote_parser_utils.c
int		get_next_separator(char *str, char sep);
int		get_first_coming_separator(char *str);
char	what_quotes(int type);
char	*what_quotes_str(int type);
int		what_flag(char c);

//expansor_2.c
void	expansor_advance(t_expansor_vars *v);

#endif
