/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:30:48 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/24 09:08:24 by igomez-s         ###   ########.fr       */
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

void	child1(char **argv, char **envp, int pipefd[2], int infile)
{
	char *cmd;

	close(pipefd[0]);
	cmd = find_path(*ft_split(argv[2], ' '), envp);
	dup2(infile, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execve(cmd, ft_split(argv[2], ' '), envp);
	perror("execve failed");
}

void	child2(char **argv, char **envp, int pipefd[2], int outfile)
{
	char *cmd;

	close(pipefd[1]);
	cmd = find_path(*ft_split(argv[3], ' '), envp);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	dup2(outfile, STDOUT_FILENO);
	execve(cmd, ft_split(argv[3], ' '), envp);
	perror("execve failed");
}

int	main(int argc, char **argv, char **envp)
{
	int	pid;
    int	pipefd[2];

	if (argc == 5)
	{
		int infile = open(argv[1], O_RDONLY);
		int outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		pipe(pipefd);
		pid = fork();
		if (pid < 0) 
		{
			perror("fork failed");
			return 1;
		}
		if (pid == 0)
        	child1(argv, envp, pipefd, infile);
		//wait(NULL);
		//waitpid(pid, NULL, 0);
		pid = fork();
		if (pid < 0) 
		{
			perror("fork failed");
			return 1;
		}
		if (pid == 0)
			child2(argv, envp, pipefd, outfile);
		wait(NULL);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	//waitpid(pid, NULL, 0);
	wait(NULL);

	return (0);
}
