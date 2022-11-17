#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include <fcntl.h>
/**
 * _strdup - same as strdup
 * @str: the string to duplicate
 * Return: the address of the duplicate
 */

char *_strdup(char *str)
{
	char *s;
	int i = 0;

	if (!str)
		return (NULL);
	s = malloc(_strlen(str) + 1);
	if (!s || sizeof(s) < sizeof(str))
		return (NULL);
	while (str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

/**
 * rem_comment - replaces the comment char '#' with '\n'
 * @s: the line
 * Return: the address of the edited string
 */

char *rem_comment(char *s)
{
	int i = 0;

	while (s[i])
	{
		if (s[i] == '#')
		{
			s[i] = '\n';
		}
		i++;
	}
	return (s);
}

/**
 * print_a - prints array of strings
 * @args: the strings
 */

void print_a(char **args)
{
	int i = 0;

	while (args[i])
	{
		_puts(args[i]);
		i++;
	}
	i = 0;
	free(args);
}

/**
 * fill_alias - populate aliases array
 * @als: aliases
 * @fd: filedes
 * Return: array of strings or NULL if failed
 */

char **fill_alias(char **als, int fd)
{
	char line[MAX_L];
	ssize_t readno, i = 0;

	while ((readno = _readline(fd, line, MAX_L)))
		if (has_alias(line))
		{
			char *temp = _strdup(line);

			als[i] = temp;
			i++;
		}
	als[i] = NULL;
	close(fd);
	return (als);
}

/**
 * has_alias - checks whether line contain the chars 'alias'
 * @line: the buffer
 * Return: 1 if yes and 0 o.w.
 */

int has_alias(char *line)
{
	int pos_a = -1, pos_e = -1, pos_h = -1;
	char *temp;

	if (has_char(line, '='))
		pos_e = _strtok(line, "=");
	if (has_char(line, '#'))
		pos_h = _strtok(line, "#");
	temp = _strstr(line, "alias");
	if (temp)
		pos_a = temp - line;
	if ((pos_e > pos_a && pos_h > pos_e) || (pos_h == -1 && pos_e > pos_a))
		return (1);
	return (0);
}
