#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "text.h"
#include "general.h"

/**
 * printenv - prints the environment
 */

void printenv(void)
{
	size_t i = 0;

	while (environ[i])
	{
		print(environ[i]);
		print("\n");
		i++;
	}
}

/**
 * _setenv - sets an env variable
 * @info: variable's key
 * @args: variable's value
 */

void _setenv(general_t *info, char **args)
{
	size_t i = 0;
	int n = 0;
	char *temp, *name, *aux;
	(void) info;

	name = args[1];
	if (!name || count(args) > 3)
		return;
	if (count(args) == 2)
		printenv();
	while (environ[i])
	{
		temp = _strdup(environ[i]);
		aux = strtok(temp, "=");
		n = _strcmp(aux, name);
		if (n == 0)
		{
			_strcat(aux, "=");
			_strcat(aux, args[2]);
			environ[i] = aux;
			printenv();
		}
		free(temp);
		i++;
	}

	if (_putenv(args))
		printenv();
}

/**
 * _putenv - puts a new env variable
 * @args: variable's key
 * Return: 1 on success and 0 o.w.
 */

int _putenv(char **args)
{
	char *new;
	size_t env_size;

	if (!environ)
		return (0);
	new = _strdup(args[1]);
	env_size = count(environ);
	if (new)
	{
		_strcat(new, "=");
		_strcat(new, args[2]);
		environ[env_size++] = new;
		environ[env_size] = NULL;
		return (1);
	}
	return (0);
}

/**
 * _unsetenv - unsets an env variable
 * @info: variable's key
 * @args: args
 */

void _unsetenv(general_t *info, char **args)
{
	size_t i = 0, found = 0;
	int n = 0;
	char *temp, *name, *aux;
	(void) info;

	name = args[1];
	if (!name || count(args) > 2)
		return;
	while (environ[i])
	{
		temp = strdup(environ[i]);
		aux = strtok(temp, "=");
		n = _strcmp(aux, name);

		if (n == 0)
		{
			found = 1;
			environ[i] = environ[i + 1];
		}
		else if (found)
		{
			environ[i] = environ[i + 1];
		}
		i++;
	}
}
