#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_files
{
	char			*name;
	struct s_files	*next;
	int				append;
}	t_files;

typedef struct s_cmd
{
	char 			*cmd; // primer par√metro de execve
	char 			**cmd_complete; //segundo parametro de execve
	t_files 		**stdins; //nombres de archivo de redirecciones stdin
	t_files 		**stdouts; //nombres de archivo de redirecciones stout
	t_files 		**stderrs; //nombres de archivo de redirecciones sterr
	t_files			**heredocs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_history
{
	char *cmd_line;
	struct s_history *next;
}	t_history;

typedef struct s_data
{
	int			num_cmds;
	int			num_pipes;
	char		**env;
	t_history	**history;
	t_cmd		**cmds;
}	t_data;
char	**ft_split(char const *s, char c);
void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new);
void	ft_lstadd_back_files(t_files **lst, t_files *new);
char	*ft_strdup_space(const char *s1);
char	*ft_strdup(const char *s1);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);
t_cmd	*ft_lstlast_cmd(t_cmd *lst);
t_files	*ft_lstlast_files(t_files *lst);

// lexer_setters.c
void	add_infile(char *raw_infile, t_cmd *raw_cmd);
void	add_outfile(char *raw_infile, t_cmd *raw_cmd, int append);
void	add_heredoc(char *raw_keyword, t_cmd *raw_cmd);



