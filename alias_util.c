#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"
#include <sys/stat.h>
#include <fcntl.h>
/**
 * handle_alias - func that handles anything alias
 * @args: cmd line args
 */

void handle_alias(char **args)
{
	size_t als_s = 0;
	int fd = -1, which;
	char **aliases, line[MAX_L];

	if (!args || _strcmp("alias", args[0]) != 0)
		return;
	if (access("/root/.bashrc", F_OK | R_OK | W_OK) == 0)
	{	which = 1;
		fd = open("/root/.bashrc", O_RDWR);
	}
	else if (access("/root/.tcshrc", F_OK | W_OK | R_OK) == 0)
	{
		which = 2;
		fd = open("/root/.bashrc", O_RDWR);
	}
	else
	{	write(2, "Error: Alias file not found\n", 28);
		return;
	}
	if (fd != -1)
	{
		while (_readline(fd, line, MAX_L))
		{
			if (has_alias(line))
				als_s++;
		}
		close(fd);
	}
	fd = which == 1 ? open("/root/.bashrc", O_RDWR)
		: open("/root/.tcshrc", O_RDWR);
	aliases = malloc(sizeof(char *) * als_s + 1);
	fill_alias(aliases, fd);
	count(args) == 1 ? print_a(aliases) : handle_args(aliases, args, which);
}

/**
 * handle_args - handle args from cmd
 * @as: aliases array
 * @args: cmd line args
 * @which: which file
 */

void handle_args(char **as, char **args, int which)
{
	int st = 0, pos;
	size_t i = 1, j;

	while (args[i] && st < 2)
	{
		if (!has_char(args[i], '='))
		{
			j = 0;
			while (as[j])
			{
				pos = _strtok(as[j], args[i]);
				if (pos < (int) _strlen(as[j]))
					_puts(as[j]);
				j++;
			}
		}
		else
		{
			set_alias(as, args[i], which);
		}
		i++;
	}
}

/**
 * set_alias - sets a new alias
 * @as: aliases array
 * @arg: argument
 * @which: which file
 * Return: 1 on success and 0 o.w.
 */

int set_alias(char **as, char *arg, int which)
{
	char *name;
	int pos_n, pos_a, pos_e, pos_s, fd;
	size_t i = 0, ln;

	fd = which == 1 ? open("~/.bashrc", O_RDWR) : open("~/.tcshrc", O_RDWR);
	pos_s = _strtok(arg, " ");
	pos_e = _strtok(arg, "=");
	name = malloc(pos_e - pos_s + 1);
	if (!name)
		return (0);
	_strncpy(name, arg + pos_s, pos_e - pos_s);
	rem_space(name);
	pos_n = -1;
	while (as[i])
	{	pos_n = _strtok(as[i], name);
		pos_e = _strtok(as[i], "=");
		pos_a = _strtok(as[i], "'");
		ln = _strlen(name);
		if ((int) (pos_n + ln + 1) == pos_e && pos_e + 1 == pos_a)
			pos_n = update_alias(name, fd);
		else if (pos_n < (int) _strlen(as[i]))
			write(2, "Usage: alias name='value'\n", 26);
	}
	if (pos_n > -1)
		return (pos_n);
	fd = which == 1 ? open("~/.bashrc", O_RDWR, O_APPEND)
		: open("~/.tchshrc", O_RDWR, O_APPEND);
	if (put_alias(name, arg, pos_e, fd))
		return (1);
	return (0);
}

/**
 * update_alias - updates alias
 * @name: alias name
 * @fd: filedes
 * Return: 1 on success and 0 o.w.
 */

int update_alias(char *name, int fd)
{
	char **buf, line[MAX_L], *temp;
	size_t lc = 0, readno, i = 0;
	int pos_n, pos_e;

	while (read(fd, line, MAX_L))
		lc++;
	buf = malloc(sizeof(char *) * (lc + 1));
	readno = 0;
	while ((readno = read(fd, line, MAX_L)))
	{
		pos_n = _strtok(line, name);
		pos_e = _strtok(line, "=");
		if (has_alias(line) && pos_n < (int) readno && pos_n < pos_e)
		{	temp = malloc(readno + 1);
			if (!temp)
				return (0);
			_strncpy(temp, line, readno);
			buf[i] = temp;
			i++;
			continue;
		}
		temp = malloc(readno + 1);
		if (!temp)
			return (0);
		_strncpy(temp, line, readno);
		buf[i] = temp;
		i++;
	}

	buf[i] = NULL;
	i = 0;
	while (buf[i])
	{	write(fd, buf[i], _strlen(buf[i]));
		i++;
	}
	close(fd);
	return (1);
}

/**
 * put_alias - puts new alias
 * @name: name of alias
 * @arg: its value
 * @pos_e: position of =
 * @fd: filedes
 * Return: 1 on success and 0 o.w.
 */

int put_alias(char *name, char *arg, int pos_e, int fd)
{
	char *line;
	size_t vl, nl;

	vl = _strlen(arg + pos_e);
	nl = _strlen(name);
	line = malloc(nl + vl + 8);
	if (!line)
		return (0);
	_strncpy(line, "alias ", 6);
	_strcat(line, name);
	_strcat(line, "=");
	_strcat(line, arg + pos_e);
	if (write(fd, line, _strlen(line)) > 0)
		return (1);
	close(fd);
	return (0);
}
