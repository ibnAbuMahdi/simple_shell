#include "text.h"
#include "commands.h"
#include "general.h"
#include "memory.h"

/**
 * prompt - Print the prompt
 *
 * @info: Struct of general information
 **/
void prompt(general_t *info)
{
	if (info->mode == NON_INTERACTIVE)
		return;

	print("$ ");
}

/**
 * read_prompt - Read lines in the prompt
 *
 * Return: Buffer readed or NULL if EOF was found
 **/
char *read_prompt()
{
	char *buf;
	int res;
	size_t size;

	buf = NULL;

	res = getline(&buf, &size, stdin);

	if (res == EOF)
	{
		free_memory_p((void *) buf);
		return (NULL);
	}

	return (buf);
}

/**
 * start_prompt - Loop reading text
 *
 * @info: Struct of general information
 *
 * Return: Buffer readed or NULL if EOF was found
 **/
void start_prompt(general_t *info)
{
	char **args, *buff, *temp, *temp1, *temp2, *path, **and_args, **or_args;
	size_t i, or_s, and_s;

	signal(SIGINT, sigintHandler);
	while (1)
	{
		prompt(info);
		path = _getenv("PATH");
		is_current_path(path, info);
		info->environment = path;
		buff = read_prompt();
		if (buff == NULL)
		{
			print(info->mode == INTERACTIVE ? "exit\n" : "");
			free(path);
			break;
		}
		info->n_commands++;
		if (buff[0] != '\n')
		{
			i = 0;
			args = split_words(buff, ";");
			while (args[i])
			{
				temp = _strdup(args[i]);
				temp1 = _strdup(args[i]);
				temp2 = _strdup(args[i]);
				and_args = split_words(temp, "&&");
				or_args = split_words(temp1, "||");
				or_s = count(or_args);
				and_s = count(and_args);
				or_s == 1 && and_s == 1 ? process_cmd(info, args[i]) :
					process_log_cmd(and_args, or_args, info, temp2);
				i++;
				free_memory_pp((void *) and_args);
				free_memory_pp((void *) or_args);
				free(temp);
				free(temp1);
			}
			free_memory_pp((void *) args);
		}
		free_memory_p((void *) buff);
		free_memory_p((void *) path);
	}
}

/**
 *
 */

void process_cmd(general_t *info, char *args)
{
	char **arguments;

	arguments = split_words(args, " \t\n");
	info->arguments = arguments;
	info->buffer = args;
	analyze_patterns(info, arguments);
	analyze(arguments, info, args);

	free_memory_pp((void *) arguments);
}

/**
 * sigintHandler - Handler for SIGINT
 *
 * @sig_num: Unused parameter
 **/
void sigintHandler(int sig_num)
{
	(void) sig_num;

	signal(SIGINT, sigintHandler);
	print("\n$ ");
	fflush(stdout);
}
