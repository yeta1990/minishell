#include <stdlib.h>
typedef struct s_files
{
	char			*name;
	struct s_files	*next;
}	t_files;

typedef struct s_cmd
{
	char 			*cmd;
	char 			**cmd_complete;
	t_files 		**stdins; //nombres de archivo de redirecciones stdin
	t_files 		**stdout; //nombres de archivo de redirecciones stout
	t_files 		**stderr; //nombres de archivo de redirecciones sterr
	int				heredoc;
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
char	*ft_strdup(const char *s1);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);