/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crisfern <crisfern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 12:45:52 by albgarci          #+#    #+#             */
/*   Updated: 2022/01/25 16:02:54 by albgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
		if (*s++ == (char)c)
			return ((char *)--s);
	if ((char)c == 0)
		return ((char *)s);
	return (0);
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)malloc(size * count);
	if (ptr)
		ft_bzero(ptr, size * count);
	return (ptr);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len-- > 0)
		*ptr++ = (unsigned char)c;
	return (b);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	if (s)
	{
		if (start < ft_strlen(s))
		{
			i = 0;
			if (len > ft_strlen(s) - start)
				len = ft_strlen(s) - start;
			ptr = ft_calloc(len + 1, 1);
			if (ptr)
			{
				while ((len-- > 0) && (s + start + i++))
					*(ptr + i - 1) = *(s + start + i - 1);
				*(ptr + i) = 0;
				return (ptr);
			}
		}
		else
			return (ft_strdup(""));
	}
	return (0);
}
