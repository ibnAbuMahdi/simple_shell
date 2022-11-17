#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/**
 * words - counts the number of words in a string delimited by needle
 * @s: the string
 * @needle: the delimiter
 * Return: the word count
 */

int words(char *s, char *needle)
{
	int wc = 0;
	char *dup = s, *temp;

	if (!s || !needle)
		return (-1);
	while ((temp = _strstr(dup, needle)))
	{
		if (_strncmp(dup, needle, _strlen(needle)) != 0)
			wc++;
		dup = temp + 1;
	}
	if (*dup)
		wc++;
	return (wc);
}

/**
 * split: splits a string into words delimited by delim
 * @str: the string
 * @delim: the delimiter
 * Return: address of the array of words
 */

char **split(char *str, char *delim)
{
	char **temp;
	char *strp, *s = str;
	size_t i = 0, dl;
	int pos;

	if (!str || !delim)
		return (NULL);
	temp = malloc((sizeof(char *)) * (words(str, delim) + 1));
	dl = _strlen(delim);
	if (!temp)
		return (NULL);

	while ((pos = _strtok(s, delim)))
	{
		strp = malloc(pos + 1);
		if (!strp)
			return (NULL);
		_strncpy(strp, s, pos);
		temp[i] = strp;
		i++;
		s += pos + dl;
	}
	temp[i] = NULL;
	return (temp);
}

/**
 * has_char - checks for char c in string s
 * @s: the string to check in
 * @c: the char
 * Return: 1 if c exist in s and 0 if not
 */

int has_char(char *s, char c)
{
	while (*s)
		if (*s++ == c)
			return (1);
	return (0);
}

/**
 * _puts - prints a string
 * @s: the string
 */

void _puts(char *s)
{
	int i = 0;

	while (s[i])
		_putchar(s[i++]);
}

/**
 * _putchar - prints a char to the stdout
 * @c: the char to print
 * Return: 1 if success else -1
 */

int _putchar(char c)
{
	return (write(1, &c, 1));
}



