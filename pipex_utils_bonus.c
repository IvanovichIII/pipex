#include "pipex_bonus.h"

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

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	close_files(int *pipefd, int infile, int outfile)
{
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);
	close(outfile);
}