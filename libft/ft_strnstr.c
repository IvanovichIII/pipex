/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:58:56 by ivan              #+#    #+#             */
/*   Updated: 2024/03/11 11:44:59 by igomez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = ft_strlen(s2);
	if (*s2 == '\0')
		return ((char *)s1);
	while (*s1 != '\0' && n >= i)
	{
		if (ft_strncmp(s1, s2, i) == 0)
			return ((char *)s1);
		s1++;
		n--;
	}
	return (NULL);
}
