#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#include "general.h"
#include "text.h"
#include "error.h"
#include "builtins.h"
#include "memory.h"

extern char **environ;

/* commands.c */
void analyze(char **arguments, general_t *info, char *buff);
void process_log_cmd(char **, char **, general_t *, char *);
int done(char *, char **);
void add_to_done(char **, char *);

/* permissions.c */
int is_executable(char *filename);
int is_file(char *command);

/* environment.c */
char *_getenv(const char *name);
char *which(char *filename, general_t *info);
void is_current_path(char *path, general_t *info);
void get_full_env(void);
/*void printenv(void);
*/

/* execute.c */
void execute(char *commands, char **arguments, general_t *info, char *buff);
int current_directory(char *cmd, char **arguments, char *buff,
		general_t *info);

/* text.c */
void process_cmd(general_t *, char *);

#endif /* COMMANDS_H */
