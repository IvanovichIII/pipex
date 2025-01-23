/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:18:03 by ivan              #+#    #+#             */
/*   Updated: 2024/03/10 00:28:12 by ivan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const char *str, int c, size_t n)
{
	while (n > 0)
	{
		if (*str == (char)c)
			return ((void *)str);
		n--;
		str++;
	}
	return (NULL);
}
