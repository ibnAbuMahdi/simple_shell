#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shell.h"
#include <sys/stat.h>
/**
 * main - driver code
 * @ac: args count
 * @av: args
 * Return: Always 0.
 */

int main(int ac, char **av)
{
	size_t n;
	char **argv, *line;

	if (ac > 3)
	{
		write(STDERR_FILENO, "Usage: ./shell cmd [filename]", 29);
	}
	while (1)
	{
		write(1, "$ ", 2);

		if (getline(&line, &n, stdin) == -1)
			exit(1);
		rem_comment(line);
		argv = split(line, "\n");
		process_cmds(argv[0], av[0]);
	}

	return (0);
}

/**
 * process_cmds - central commands processing center
 * @line: the cmds string with only \n removed
 * @prog: prog name
 */

void process_cmds(char *line, char *prog)
{
	char **cmds, **args;
	size_t i = 0;
	struct stat st;

	cmds = split(line, ";");
	while (cmds[i])
	{
		cmds[i] = rem_space(cmds[i]);
		if (builtin(cmds[i]))
		{	i++;
			continue;
		}

		args = split(cmds[i], " ");
		if (has_char(args[0], '/'))
		{
			if (!stat(args[0], &st))
				exec_cmd(args[0], args, prog);
			else
				write(2, "Error: Not a valid command\n", 27);
		}
		else
		{
			args[0] = cmd_exist(args[0]);
			if (args[0])
				exec_cmd(args[0], args, prog);
			else
				write(2, "Error: Not a valid command\n", 27);
		}
		i++;
	}
}
/**
 * exec_cmd - executes the command cmd with args
 * @cmd: basically args[0]
 * @args: the args
 * @prog: prog name
 * Return: 1 on success and 0 o.w.
 */

int exec_cmd(char *cmd, char **args, char *prog)
{
	pid_t child = fork();

	if (child == -1)
	{
		perror(prog);
		return (0);
	}
	else if (child == 0)
	{
		if (execve(cmd, args, environ) == -1)
		{
			perror(prog);
			return (0);
		}
	}
	else
	{
		wait(NULL);
		free(cmd);
		return (1);
	}
	return (0);
}

/**
 * cmd_exist - checks the PATH to determine if cmd exist
 * @cmd: the command
 * Return: a complete path cmd if cmd exist and null o.w.
 */

char *cmd_exist(char *cmd)
{
	char *value = _getenv("PATH");
	struct stat st;
	int pos;
	int i;
	char *temp;

	if (value)
	{
		while (1)
		{
			pos = _strtok(value, ":");

			temp = malloc(pos + 1);
			_strncpy(temp, value, pos + 1);
			_strcat(temp, "/");
			_strcat(temp, cmd);
			i = stat(temp, &st);
			if (i == 0)
			{
				return (temp);
			}
			value += pos + 1;
			pos = _strtok(value, ":");
			if (!pos)
			{
				free(temp);
				break;
			}
			free(temp);
		}
	}

	return (NULL);
}
