/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:30:48 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/23 21:55:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv)
{
    char    **env;
    int     i;

    env = ft_split(getenv("PATH"), ':');
    i = 0;
    while (env[i])
    {
        ft_printf("%s\n", env[i]);
        i++;
    }
    return (0);
}