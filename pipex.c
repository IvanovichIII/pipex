/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:30:48 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/25 12:58:09 by igomez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	return (0);
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
	return (0);
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
