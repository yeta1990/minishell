#include "minishell.h"

void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*aux;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	aux = ft_lstlast_cmd(*lst);
	aux->next = new;
}

t_cmd	*ft_lstlast_cmd(t_cmd *lst)
{
	t_cmd	*aux;

	if (!lst)
		return (0);
	aux = lst;
	while (aux->next)
		aux = aux->next;
	return (aux);
}

t_files	*ft_lstlast_files(t_files *lst)
{
	t_files	*aux;

	if (!lst)
		return (0);
	aux = lst;
	while (aux->next)
		aux = aux->next;
	return (aux);
}

void	ft_lstadd_back_files(t_files **lst, t_files *new)
{
	t_files	*aux;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	aux = ft_lstlast_files(*lst);
	aux->next = new;
}

