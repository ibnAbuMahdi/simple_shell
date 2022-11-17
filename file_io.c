#include "shell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * _readline - reads line from filedes fd
 * @fd: filedes
 * @line: buffer
 * @s: size
 * Return: chars read
 */

ssize_t _readline(int fd, char *line, size_t s)
{
	ssize_t i = 0;

	if (fd < 0 || !line || !s)
		return (-1);
	while ((size_t) i < s && read(fd, line + i, 1))
		i++;
	line[i] = '\0';
	return (i);
}
