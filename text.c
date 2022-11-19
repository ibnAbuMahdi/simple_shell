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
	char **arguments, **args, *buff, *path;
	size_t i;

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
				arguments = split_words(args[i], " \t\n");
			
				info->arguments = arguments;
				info->buffer = args[i];
				analyze_patterns(info, arguments);
				analyze(arguments, info, args[i]);

				free_memory_pp((void *) arguments);
				i++;
			}
			free_memory_pp((void *) args);

		}

		free_memory_p((void *) buff);
		free_memory_p((void *) path);
	}

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
