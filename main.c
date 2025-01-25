/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:30:48 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/25 12:58:09 by igomez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *cmd, char **envp)
{
	char	**env;
	char	*path;
	int		i;
	int		j;

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
				if (access(path, F_OK) == NO_ERROR)
					return (path);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}

int	child1(char **argv, char **envp, int pipefd[2], int infile)
{
	char	*cmd;
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		cmd = find_path(*ft_split(argv[2], ' '), envp);
		dup2(infile, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execve(cmd, ft_split(argv[2], ' '), envp);
		perror("execve failed");
	}
}

int	child2(char **argv, char **envp, int pipefd[2], int outfile)
{
	char	*cmd;
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[1]);
		cmd = find_path(*ft_split(argv[3], ' '), envp);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		dup2(outfile, STDOUT_FILENO);
		execve(cmd, ft_split(argv[3], ' '), envp);
		perror("execve failed");
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	pipefd[2];
	int	infile;
	int	outfile;

	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		pipe(pipefd);
		if (child1(argv, envp, pipefd, infile) == 1)
			return (1);
		if (child2(argv, envp, pipefd, outfile) == 1)
			return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	wait(NULL);
	wait(NULL);
	return (0);
}
