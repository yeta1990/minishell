/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_full_string.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 10:18:26 by albgarci          #+#    #+#             */
/*   Updated: 2021/11/30 15:04:31 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_first_oc(char *s1, char *set);
static int	ft_last_oc(char *s1, char *set);

char	*ft_strtrim_full_str(char const *s1, char const *set)
{
	char	*s;
	int		first_oc;
	int		last_oc;

	if (ft_strlen(set) == 0)
		return ((char *) s1);
	if (!s1 || ft_strlen(s1) == 0)
		return (0);
	s = NULL;
	first_oc = ft_first_oc((char *) s1, (char *) set);
	last_oc = ft_last_oc((char *) s1, (char *) set);
	s = ft_substr(s1, first_oc, ft_strlen(s1) - first_oc - last_oc);
	return (s);
}

static int	ft_first_oc(char *s1, char *set)
{
	size_t	i;

	i = 0;
	if (!s1 || !set)
		return (0);
	if (s1[i] == set[i])
	{
		while (set[i])
		{
			if (set[i] != s1[i])
				break ;
			i++;
		}
	}
	if (i == ft_strlen(set))
		return (i);
	return (0);
}

static int	ft_last_oc(char *s1, char *set)
{
	size_t	i;
	size_t	j;

	i = ft_strlen(s1) - ft_strlen(set);
	j = 0;
	if (s1[i] == set[j] && i > 0)
	{
		while (set[j])
		{
			if (set[j] != s1[i + j])
			{
				j = 0;
				break ;
			}
			j++;
		}
	}
	if (j == ft_strlen(set))
		return (j);
	return (0);
}
