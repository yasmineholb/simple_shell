#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>

#define BUFFER_LEN 1024
char *concat_all(char *s1, char *s2);
char *find_path(char *av);
void tokenize(char *line, char **argvv, int bufsize);
int check_builtin(char *argvv);
void print_env(void);
void builtins(char **argvv, int i);
void execute(char **argvv);
int main(void);