#include "commands.h"
#include "builtins.h"
#include "general.h"
#include "memory.h"
#include "text.h"
#include <stdio.h>

/**
 * analyze - Analyze	the arguments
 *
 * @arguments: Commands and arguments to execute
 * @info: General information about the shell
 * @buff: Line readed
 **/
void analyze(char **arguments, general_t *info, char *buff)
{
	char *cmd;
	int status;

	if (*arguments == NULL || arguments == NULL)
		return;

	cmd = arguments[0];
	info->command = cmd;
	if (check_builtin(info, arguments) == _TRUE)
		return;

	status = is_file(cmd);
	if (status == NON_PERMISSIONS)
	{
		info->status_code = 126;
		info->error_code = _CODE_EACCES;
		error(info);
		return;
	}

	if (status == 1)
	{
		execute(cmd, arguments, info, buff);
		return;
	}

	if (current_directory(cmd, arguments, buff, info) == _TRUE)
		return;

	info->value_path = which(cmd, info);
	if (info->value_path != NULL)
	{
		execute(info->value_path, arguments, info, buff);
		free_memory_p((void *) info->value_path);
		return;
	}

	info->status_code = 127;
	info->error_code = _CODE_CMD_NOT_EXISTS;
	error(info);
}

/**
 *
 */

void process_log_cmd(char **and_args, char **or_args, general_t *info, char *args)
{
	size_t and_len, or_len, and_s = _strlen(and_args[0]), or_s = _strlen(or_args[0]), max_len, a = 0, o = 0, op = 2, and_err = 1, prv;
	char **done_list, **temp, *or_p = NULL, *or_t = NULL;
	(void) args;

	and_len = count(and_args);
	or_len = count(or_args);
	max_len = and_len + or_len;
	
	done_list = malloc(sizeof(char *) * (or_len + 1));
	done_list[0] = NULL;
	while(max_len > 1)
	{
		if (op == 1)
		{
			or_s += _strlen(or_args[o]);
			temp = split_words(or_args[o], "&&");
			or_p = rem_space(temp[0]);
			or_t = _strdup(or_p);
			if (!done(or_p, done_list))
			{
				process_cmd(info, or_p);
				if (!info->status_code)
				{
					and_err = 1;
					add_to_done(done_list, or_t);
				}
				else
				{
					and_err = and_err | 0;
					info->status_code = 0;
				}
			}
			info->status_code = 0;
			o++;
			free_memory_pp((void **) temp);
			prv = 1;
		}
		else if (!op)
		{
			and_s += _strlen(and_args[a]);
			temp = split_words(and_args[a], "||");
			if (and_err)
			{
				process_cmd(info, temp[0]);
				if (info->status_code)
					and_err = 0;
			}
			a++;
			prv = 0;
			free_memory_pp((void **) temp);
		}
		else if (op == 2)
		{
			if (and_s > or_s)
			{
				or_p = rem_space(or_args[0]);
				or_t = _strdup(or_p);
				process_cmd(info, or_p);
			   	if (!info->status_code)
					add_to_done(done_list, or_t);
				if (info->status_code)
					and_err = 0;
				o++;
				op = prv = 1;
				a++;
			}
			else
			{
				or_p = rem_space(and_args[0]);
				process_cmd(info, and_args[0]);
				if (info->status_code)
					and_err = 0;
				else
					add_to_done(done_list, or_p);
				a++;
				op = prv = 0;
				o++;
			}
		}
		if (or_p) 
			free(or_p);
		if (or_t)
			free(or_t);
		or_t = or_p = NULL;
		if (and_s == or_s)
			op = prv;
		else if (a > 1 || o > 1)
			op = and_s > or_s ? 1 : 0;
		max_len--;
	}
	free_memory_pp((void **) done_list);
}

/**
 *
 */

int done(char *s, char ** list)
{
	int i = 0;

	if (!list)
		return (0);
	while (list[i])
	{
		if (_strcmp(list[i], s) == 0)
			if (_strlen(list[i]) == _strlen(s))
				return (1);
		i++;
	}
	return (0);
}

/**
 *
 */

void add_to_done(char **done_list, char *or_arg)
{
	size_t len = 0;

	while (done_list[len])
		len++;
	done_list[len] = _strdup(or_arg);
	done_list[len + 1] = NULL;
}




