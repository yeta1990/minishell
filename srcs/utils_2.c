/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:46:00 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/17 20:17:55 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	isinset(char const *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		len;
	char	*ptr;

	if (s1 && set)
	{
		while (*s1)
		{
			if (isinset(set, *s1) == 1)
				s1++;
			else
				break ;
		}
		len = ft_strlen(s1);
		while (*(s1 + len - 1) && (len >= 0))
		{
			if (isinset(set, *(s1 + len - 1)))
				len--;
			else
				break ;
		}
		ptr = ft_substr(s1, 0, len);
		return (ptr);
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	int	n;

	n = 0;
	if (!s)
		return (0);
	if (s && *s)
	{
		while (*s++)
			n++;
	}
	return (n);
}

char	*ft_strdup(const char *s1)
{
	char	*s;

	if (!s1)
		return (0);
	s = malloc(ft_strlen(s1) + 1);
	if (!s)
		return (0);
	ft_memcpy(s, s1, ft_strlen(s1) + 1);
	return (s);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	src = (char *) src;
	if (src == dst && src == 0 && n > 0)
		return (dst);
	while (i < n)
	{
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
		i++;
	}
	return (dst);
}
