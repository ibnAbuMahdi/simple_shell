#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * builtin - check if cmd is a built-in
 * @cmd: the command
 * Return: >0 if builtin else 0
 */

int builtin(char *cmd)
{
	size_t index;
	char **cmd_b;

	cmd_b = split(cmd, " ");
	index = cmd_index(cmd_b[0]);
	switch (index)
	{
		case (1):
			printenv();
			break;
		case (2):
			handle_exit(cmd_b);
			break;
		case (3):
		{
			if (count(cmd_b) != 3)
			{
				write(2, "Usage: setenv VARIABLE VALUE\n", 29);
				break;
			}
			_setenv(cmd_b[1], cmd_b[2]);
			break;
		}
		case (4):
		{
			if (count(cmd_b) != 2)
			{
				write(2, "Usage: unsetenv VARIABLE\n", 25);
				break;
			}
			_unsetenv(cmd_b[1]);
			break;
		}
		case (5):
			handle_alias(cmd_b);
	}
	return (index);
}

/**
 * cmd_index - find the index of cmd in list of builtins
 * @cmd: the command
 * Return: the index, starting from 1 or 0 if not exist
 */

size_t cmd_index(char *cmd)
{
	char *cmds_b[] = {"env", "exit", "setenv", "unsetenv", "alias", NULL};
	size_t i = 0;

	while (cmds_b[i])
	{
		if (_strcmp(cmds_b[i], cmd) == 0)
			return (i + 1);
		i++;
	}
	return (0);
}
/**
 * count - counts the number of string array elements
 * @array: string array
 * Return: array size
 */

size_t count(char **array)
{
	size_t i = 0;

	if (!array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

/**
 * handle_exit - handles the exit command
 * @args: arguments
 */

void handle_exit(char **args)
{
	size_t c;
	char *err = NULL;
	int status = -1;

	if (!args || _strcmp("exit", args[0]) != 0)
		return;
	c = count(args);
	if (c > 2)
		write(2, "Usage: exit [STATUS]\n", 21);
	if (c == 1)
		exit(0);
	if (c == 2)
	{
		status = _atol(args[1], &err);
		if (!err)
			exit(status);
		write(2, "Error: STATUS should be a number\n", 33);
	}
}

