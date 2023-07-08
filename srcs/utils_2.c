/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgarci <albgarci@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:46:00 by albgarci          #+#    #+#             */
/*   Updated: 2023/07/08 18:24:08 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s;
	size_t	i;
	size_t	j;

	i = 0;
	if (!s1 || !set)
		return (0);
	j = ft_strlen(s1);
	if (!set)
	{
		s = ft_strdup(s1);
		if (!s)
			return (0);
		return (s);
	}
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (j > i && ft_strrchr(set, s1[j]))
		j--;
	s = ft_substr((char *)s1, i, j - i + 1);
	if (!s)
		return (0);
	return (s);
}

size_t	ft_strlen(const char *s)
{
	int	n;

	n = 0;
	if (!s || !*s)
		return (0);
	if (s && *s)
	{
		while (s && *s++)
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
