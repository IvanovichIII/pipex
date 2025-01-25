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

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

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
			if (env)
				free_split(env);
		}
		i++;
	}
	return (NULL);
}

int	child1(char **argv, char **envp, int pipefd[2], int infile)
{
	char	*cmd;
	int		pid;
	char	**cmd_split;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		cmd_split = ft_split(argv[2], ' ');
		cmd = find_path(*cmd_split, envp);
		dup2(infile, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execve(cmd, cmd_split, envp);
		free(cmd);
		free_split(cmd_split);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

int	child2(char **argv, char **envp, int pipefd[2], int outfile)
{
	char	*cmd;
	int		pid;
	char	**cmd_split;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[1]);
		cmd_split = ft_split(argv[3], ' ');
		cmd = find_path(*cmd_split, envp);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		dup2(outfile, STDOUT_FILENO);
		execve(cmd, cmd_split, envp);
		free(cmd);
		free_split(cmd_split);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	close_files(int *pipefd, int infile, int outfile)
{
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);
	close(outfile);
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
		if (infile < 0 || outfile < 0)
			return (1);
		if (pipe(pipefd) < 0)
			return (1);
		if (child1(argv, envp, pipefd, infile) == 1)
			return (1);
		if (child2(argv, envp, pipefd, outfile) == 1)
			return (1);
	}
	close_files(pipefd, infile, outfile);
	wait(NULL);
	wait(NULL);
	return (0);
}
