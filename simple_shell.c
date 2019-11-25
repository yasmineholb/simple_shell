#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define BUFFER_LEN 1024

/**
 * concat_all - concatenate two strings
 * @s1: string
 * @s2: string
 *Return: string
 */
char *concat_all(char *s1, char *s2)
{
	char *result = NULL;
	int l1 = 0, l2 = 0, i = 0, k = 0;

for (l1 = 0; s1[l1]; l1++)
;

for (l2 = 0; s2[l2]; l2++)
;

	result = malloc(sizeof(char) * (l1 + l2 + 1));
	if (!result)
		return (NULL);

	for (i = 0; s1[i]; i++)
		result[i] = s1[i];
	k = i;

	for (i = 0; s2[i]; i++)
		result[k + i] = s2[i];
	k = k + i;

	result[k] = '\0';

	return (result);
}



/**
 * find_path - find the path of the command
 * @av: the command to look for its path
 *Return: string: full path of the command
 */
char *find_path(char *av)
{
char *path = NULL, *token = NULL, *cpath = NULL;
int i = 0, len = 0;
struct stat sfile;

path = getenv("PATH");
for (len = 0; path[len]; len++)
;

cpath = malloc(sizeof(char) * len + 1);

for (i = 0; path[i]; i++)
cpath[i] = path[i];
cpath[i] = '\0';



token = strtok(cpath, ":");
token = concat_all(token, "/");
token = concat_all(token, av);
while (token != NULL)
  {
    if (stat(token, &sfile) == 0)
		return (token);

	token = strtok(NULL, ":");
	if (token != NULL)
	{
  token = concat_all(token, "/");
  token = concat_all(token, av);
	printf("%s\n", token);
}

}

  return (av);
}

/**
 * tokenize - parse the input string
 * @line: string
 * @argvv: array of strings to fill
 * @bufsize: size of buffer for allocation
 *Return: void
 */
void tokenize(char *line, char **argvv, int bufsize)
{
	char *token = NULL;
	int	i = 0;

	token = strtok(line, " ");
	while (token != NULL)
	{
		argvv[i] = token;
		token = strtok(NULL, " ");
		i++;
		if (i >= bufsize)
		{
			bufsize += BUFFER_LEN;
			argvv = realloc(argvv, bufsize * sizeof(char *));
			if (!argvv)
			{
				perror("tokenize fail");
				exit(EXIT_FAILURE);
			}
		}
	}
	argvv[i] = '\0';
}

void helpsection()
{
	puts("\n*** SHELL HELP builtins*** "
			"\n-These shell commands are defined internally"
			"\n-Use the shell at your own risk..."
			"\nList of Commands supported:"
			"\n>cd"
			"\n>ls"
			"\n>exit"
			"\n>env"
			"\n>all other general commands available in UNIX shell"
			"\n>pipe handling"
			"\n>improper space handling");
	return;
}
/* Function to execute builtin commands */
int builtins(char** argvv)
{
	int i, k = 0;
	char* cm[4];
	char* username;
	cm[0] = "exit";
	cm[1] = "cd";
	cm[2] = "help";
	cm[3] = "env";
	for (i = 0; i < 4; i++) {
		if (strcmp(argvv[0], cm[i]) == 0) {
			k = i + 1;
			break;
		}
	}
	switch (k) {
		case 1:
			printf("\nGoodbye\n");
			exit(0);
		case 2:
			chdir(argvv[1]);
			return 1;
		case 3:
			helpsection();
			return 1;
		case 4:
			username = getenv("USER");
			printf("\nHello %s.\nMind that this is "
					"not a place to play around."
					"\nUse help to know more..\n", username);
			return 1;
		default:
			break;
	}
	return 0;
}


/**
 *main - main program for the shell project
 *
 *Return: 0
 */
int main(void)
{
	int bufsize = BUFFER_LEN, pid = 0, read;
	char **argvv = malloc(sizeof(char) * bufsize);
	char *line = NULL;
	size_t length = 0;

	if (!argvv)
	{
		perror("failed to allocate memory for tokens\n");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		printf("$ ");

		if ((read = getline(&line, &length, stdin)) == EOF)
		{
			printf("\n");
			exit(0);
		}
		if (read == -1)
		{
			fprintf(stderr, "lsh: unable to read command\n");
			exit(EXIT_FAILURE);
		}


		length = strlen(line);
		if (line[length - 1] == '\n')
			line[length - 1] = '\0';

		tokenize(line, argvv, bufsize);
/*if it's not a builtin or an alias*/
/*look for path*/

// if (find_path(argvv[0]) == NULL)
// {
// 	perror("command: not found\n");
// 	exit(127);
// }
// else
argvv[0] = find_path(argvv[0]);

// if (builtins(argvv))
// 	return 0;

		pid = fork();
		if (pid == 0)
		{
			if (execve(argvv[0], argvv, NULL) == -1)
		    {
					perror("hsh");
					return (0);
				}
		}
		else if (pid == -1)
		{
			perror("hsh error - child is -1");
exit(-1);
}
		else
			wait(NULL);
	}

	free(argvv);

	return (0);
}
