#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"

//**************************************//
//           2011 Hunter Lang           //
//**************************************// 

enum BUILTIN_COMMANDS {NO_SUCH_BUILTIN=0, EXIT,JOBS,ADD, DEL};
enum bool {False=0, True=1};
 
char* build_prompt() {
  return  "> ";
}
 
int is_builtin_command(char* cmd) {
  if (strncmp(cmd, "exit", strlen("exit")) == 0)
    return EXIT;
  else if (strncmp(cmd, "jobs", strlen("jobs")) == 0)
    return JOBS;
  else if (strncmp(cmd, "add", strlen("add")) == 0)
    return ADD;
  else if (strncmp(cmd, "delete", strlen("delete")) == 0)
	return DEL;
  return NO_SUCH_BUILTIN;
    
}

int main (int argc, char **argv)
{
    char * cmd_line;
    parse_info *info;         // information returned by parser.
    struct command_type *com; // command name and arg list for one command.

	while(1) {
#ifdef UNIX
	  cmd_line = readline(build_prompt());
	  if (cmd_line == NULL) {
		  fprintf(stderr, "Unable to read command\n");
		  continue;
	  }
#endif

	  info = parse(cmd_line);
	  if (info == NULL) {
		free(cmd_line);
		continue;
	  }
	  print_info(info);

	  // info of command before |
	  com=&info->com_array[0];
	  if ((com == NULL)  || (com->command == NULL)) {
		FILE *fp;
		char line[130];
		fp = popen("blam", "r");
		while (fgets(line, sizeof line, fp)) {
		  printf("%s", line);
		}
		pclose(fp);
		free_info(info);
		free(cmd_line);
		continue;
	  }
	  // command name of com
	  if (is_builtin_command(com->command) == EXIT) {
		exit(1);
	  }
	  else if (is_builtin_command(com->command) == JOBS) {
		printf("You asked for jobs.\n");
	  }
	  else if (is_builtin_command(com->command) == ADD) {
		if (com->var_list[1]) {
		  printf("Add to %s?\n", com->var_list[1]);
		  char * key = readline("Key:");
		  char * val = readline("Value:");
		  FILE *fp;
		  char line[130];
		  char command[60] = "blam ";
		  strncat(command, com->var_list[1], strlen(com->var_list[1]));
		  strncat(command, " ", 1);
		  strncat(command, key, strlen(key));
		  strncat(command, " ", 1);
		  strncat(command, val, strlen(val));
		  fp = popen(command, "r");
		  while (fgets(line, sizeof line, fp)) {
			printf("%s", line);
		  }
		  pclose(fp);
		  
		}
		else {
		  printf("Add what?\n");
		}
	  }
	  else if (is_builtin_command(com->command) == DEL) {
		if (com->var_list[2]) {
		  FILE *fp;
		  char line[130];
		  char command[60] = "blam ";
		  strncat(command, com->var_list[1], strlen(com->var_list[1]));
		  strncat(command, " ", 1);
		  strncat(command, com->var_list[2], strlen(com->var_list[2]));
		  strncat(command, " delete", 7);
		  fp = popen(command, "r");
		  while (fgets(line, sizeof line, fp)) {
			printf("%s", line);
		  }
		  pclose(fp);
		}
		else if (com->var_list[1]) {
		  FILE *fp;
		  char line[130];
		  char command[60] = "blam ";
		  strncat(command, com->var_list[1], strlen(com->var_list[1]));
		  strncat(command, " delete", 7);
		  fp = popen(command, "r");
		  while (fgets(line, sizeof line, fp)) {
			printf("%s", line);
		  }
		  pclose(fp);
		}


	  }
	  else {
		if (com->var_list[1]) {
		  FILE *fp;
		  char line[130];
		  char command[60] = "blam ";
		  strncat(command, com->var_list[0], strlen(com->var_list[0]));
		  strncat(command, " ", 1);
		  strncat(command, com->var_list[1], strlen(com->var_list[1]));
		  fp = popen(command, "r");
		  while (fgets(line, sizeof line, fp)) {
			printf("%s", line);
		  }
		  pclose(fp);
		}
		else if (com->var_list[0]) {
		  FILE *fp;
		  char line[130];
		  char command[60] = "blam ";
		  strncat(command, com->var_list[0], strlen(com->var_list[0]));
		  fp = popen(command, "r");
		  while (fgets(line, sizeof line, fp)) {
			printf("%s", line);
		  }
		  pclose(fp);
		}
	  }
	  free_info(info);
	  free(cmd_line);
	}
}
