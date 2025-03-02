/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igomez-s <igomez-s@student.42.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:11:43 by igomez-s          #+#    #+#             */
/*   Updated: 2025/02/27 18:20:55 by igomez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	child_head(char **argv, char **envp, int pipefd[2], int infile)
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

int	child(char *cmd_arg, char **envp, int infile, int outfile)
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
		dup2(infile, STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		close(infile);
		close(outfile);
		cmd_split = ft_split(cmd_arg, ' ');
		cmd = find_path(*cmd_split, envp);
		execve(cmd, cmd_split, envp);
		free(cmd);
		free_split(cmd_split);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	return (0);
}

int	child_end(char *argv, char **envp, int pipefd[2], int outfile)
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
		cmd_split = ft_split(argv, ' ');
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

int	pipex_bonus(int argc, char **argv, char **envp, int pipefd[2])
{
	int	infile;
	int	outfile;
	int	prev_pipe;
	int	i;

	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (infile < 0 || outfile < 0)
		return (1);
	if (pipe(pipefd) < 0)
		return (1);
	if (child_head(argv, envp, pipefd, infile) == 1)
		return (1);
	close(infile);
	close(pipefd[1]);
	prev_pipe = pipefd[0];
	i = 3;
	while (i < argc - 2)
	{
		if (pipe(pipefd) < 0)
			return (1);
		if (child(argv[i], envp, prev_pipe, pipefd[1]) == 1)
			return (1);
		close(prev_pipe);
		close(pipefd[1]);
		prev_pipe = pipefd[0];
		i++;
	}
	if (child_end(argv[argc - 2], envp, pipefd, outfile) == 1)
		return (1);
	close(prev_pipe);
	close(outfile);
	return (0);
}

void	here_doc_input(char *limiter, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	exit(0);
}

int	pipex_bonus_here(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	int		outfile;
	int		prev_pipe;
	int		i;
	pid_t	pid;

	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outfile < 0)
		return (1);
	if (pipe(pipefd) < 0)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		here_doc_input(argv[2], pipefd);
	wait(NULL);
	close(pipefd[1]);
	prev_pipe = pipefd[0];
	i = 3;
	while (i < argc - 2)
	{
		if (pipe(pipefd) < 0)
			return (1);
		if (child(argv[i], envp, prev_pipe, pipefd[1]) == 1)
			return (1);
		close(prev_pipe);
		close(pipefd[1]);
		prev_pipe = pipefd[0];
		i++;
	}
	if (child_end(argv[argc - 2], envp, pipefd, outfile) == 1)
		return (1);
	close(prev_pipe);
	close(outfile);
	return (0);
}

/*
   int pipex_bonus_here(int argc, char **argv, char **envp, int pipefd[2])
   {
   int outfile;
   int prev_pipe;
   int i;

   outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
   if (outfile < 0)
   return (1);
   if (pipe(pipefd) < 0)
   return (1);
   if (child_head(argv, envp, pipefd, STDIN_FILENO) == 1)
   return (1);
   close(pipefd[1]);
   prev_pipe = pipefd[0];
   i = 3;
   while (i < argc - 2)
   {
   if (pipe(pipefd) < 0)
   return (1);
   if (child(argv[i], envp, prev_pipe, pipefd[1]) == 1)
   return (1);
   close(prev_pipe);
   close(pipefd[1]);
   prev_pipe = pipefd[0];
   i++;
   }
   if (child_end(argv[argc - 2], envp, pipefd, outfile) == 1)
   return (1);
   close(prev_pipe);
   close(outfile);
   return (0);
   }

 */

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
		if (child_head(argv, envp, pipefd, infile) == 1)
			return (1);
		if (child_end(argv[argc - 2], envp, pipefd, outfile) == 1)
			return (1);
	}
	else if (argc > 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
			return (pipex_bonus_here(argc, argv, envp));
		else
			return (pipex_bonus(argc, argv, envp, pipefd));
	}
	close_files(pipefd, infile, outfile);
	wait(NULL);
	wait(NULL);
	return (0);
}
