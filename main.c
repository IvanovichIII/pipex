/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:30:48 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/24 01:31:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char    *find_path(char *cmd, char **envp)
{
    char    **env;
    char    *path;
    int     i;
    int     j;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            j = 0;
            env = ft_split(envp[i] + 5, ':');
            while (env[j])
            {
                path = ft_strjoin(env[j], ft_strjoin("/", cmd));
                if (access(path, F_OK) == 0)
                {
                    return (path);
                }
                j++;
            }
        }
        i++;
    }
    return (NULL);
}


int main(int argc, char **argv, char **envp)
{
    char    *cmd;

    ft_printf("hola");
    if (argc >= 2)
    {
        cmd = find_path(argv[1], envp);
        execve(cmd, argv + 1, envp);
    }
    return (0);
}