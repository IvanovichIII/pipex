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

int pipex_bonus(int argc, char **argv, char **envp, int pipefd[2])
{
    int infile;
	int outfile;
    int prev_pipe;
    int i;
    
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
        if (pipex_bonus(argc, argv, envp, pipefd) == 1)
            return (1);
	close_files(pipefd, infile, outfile);
	wait(NULL);
	wait(NULL);
	return (0);
}
