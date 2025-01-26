/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:35:07 by igomez-s          #+#    #+#             */
/*   Updated: 2025/01/25 12:52:25 by igomez-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "gnl/get_next_line.h"
# include "printf/ft_printf.h"
# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include "sys/wait.h"

# ifndef NO_ERROR
#  define NO_ERROR 0
# endif

//Utils
char	*find_path(char *cmd, char **envp);
void	free_split(char **arr);
void	close_files(int *pipefd, int infile, int outfile);

//Pipex
int	child_head(char **argv, char **envp, int pipefd[2], int infile);
int	child(char *cmd_arg, char **envp, int infile, int outfile);
int	child_end(char *argv, char **envp, int pipefd[2], int outfile);
int	main(int argc, char **argv, char **envp);

#endif
