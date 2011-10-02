#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

//**************************************//
//           2011 Hunter Lang           //
//**************************************// 

#define MAXLINE 81

void init_info(parse_info *p) {
  int i;
  p->bool_infile = 0;
  p->bool_outfile = 0;
  p->bool_background = 0;
  p->pipe_num = 0;

  for (i = 0; i < PIPE_MAX_NUM; i++) {
    p->com_array[i].command = NULL;
    p->com_array[i].var_list[0] = NULL;
    p->com_array[i].var_num = 0;
  }
}

int parse_command(char * command, struct command_type *comm) {
  int i = 0;
  int pos = 0;
  char word[MAXLINE];

  comm->var_num = 0;
  comm->command = NULL;
  comm->var_list[0] = NULL;
  while(isspace(command[i]))
    i++; 
  if (command[i] == '\0') 
    return 1;
  while (command[i] != '\0') {
    while (command[i] != '\0' && !isspace(command[i])) {
      word[pos++] = command[i++];
    }
    word[pos] = '\0';
    if (comm->var_num == MAX_VAR_NUM ) {
      fprintf(stderr, "Too many arguments to command.\n");      
      return 0;
    }
    comm->var_list[comm->var_num] = malloc((strlen(word)+1)*sizeof(char));
    strcpy(comm->var_list[comm->var_num], word);
    comm->var_num++;
    word[0] = '\0';
    pos = 0;
    while(isspace(command[i]))
      i++; 
  }
  comm->command = malloc((strlen(comm->var_list[0])+1)*sizeof(char));
  strcpy(comm->command, comm->var_list[0]);
  comm->var_list[comm->var_num] = NULL;
  return 1;
}

parse_info *parse (char *cmdline) {
  parse_info *result;
  int i = 0;
  int pos;
  int end = 0;
  char command[MAXLINE];
  int com_pos;
  int is_comm_proper = 0;

  if (cmdline[i] == '\n' && cmdline[i] == '\0')
    return NULL;

  result = malloc(sizeof(parse_info));
  if (result == NULL) {
    return NULL;
  }
  init_info(result);
  com_pos = 0;
  while (cmdline[i] != '\n' && cmdline[i] != '\0') {
    if (cmdline[i] == '&') {
      result->bool_background = 1;
      if (cmdline[i+1] != '\n' && cmdline[i+1] != '\0')
	fprintf(stderr, "Ignore anything beyond &.\n");
      break;
    }
    else if (cmdline[i] == '<') {
      result->bool_infile = 1;
      while (isspace(cmdline[++i]));
      pos = 0;
      while (cmdline[i] != '\0' && !isspace(cmdline[i])) {
	if (pos==FILE_MAX_SIZE) {
	  fprintf(stderr, "Error.The input redirection file name exceeds the size limit 40\n");
	  free_info(result);
	  return NULL;
	}
	result->in_file[pos++] = cmdline[i++];
      }
      result->in_file[pos]='\0';
      end =1;
      while (isspace(cmdline[i])) {
	if (cmdline[i] == '\n')
	  break;
	i++;
      }
    }
    else if (cmdline[i] == '>') {
      result->bool_outfile = 1;
      while (isspace(cmdline[++i]));
      pos = 0;
      while (cmdline[i] != '\0' && !isspace(cmdline[i])) {
	if (pos==FILE_MAX_SIZE) {
	  fprintf(stderr, "Error.The output redirection file name exceeds the size limit 40\n");
	  free_info(result);
	  return NULL;
	}
	result->out_file[pos++] = cmdline[i++];
      }
      result->out_file[pos]='\0';
      end =1;
      while (isspace(cmdline[i])) {
	if (cmdline[i] == '\n') 
	  break;
	i++;
      } 
    }
    else if (cmdline[i] == '|') {
      command[com_pos]='\0';
      is_comm_proper = parse_command(command, &result->com_array[result->pipe_num]);
      if (!is_comm_proper) {
	free_info(result);
	return NULL;
      }
      com_pos = 0;
      end = 0;
      result->pipe_num++;
      i++;
    }
    else {
      if (end == 1) {
		fprintf(stderr, "Error.Wrong format of input\n");
		free_info(result);
		return NULL;
      }
      if (com_pos == MAXLINE-1) {
		fprintf(stderr, "Error. The command length exceeds the limit 80\n");
		free_info(result);
		return NULL;
      }
      command[com_pos++] = cmdline[i++];
    }
  }
  command[com_pos]='\0';
  is_comm_proper = parse_command(command, &result->com_array[result->pipe_num]);
  if (!is_comm_proper) {
    free_info(result);
    return NULL;
  }
  //Result->pipeNum++;
  return result;
}

void print_info (parse_info *info) {
  int i, j;
  struct command_type *comm;

  if (NULL == info) {
    printf("Null info\n");
    return;
  }
  for (i = 0; i<=info->pipe_num;i++) {
    comm=&(info->com_array[i]);
    if ((NULL == comm) || (NULL == comm->command)) {
      //printf("Command %d is NULL\n", i);
    } 
    else 
    {
    }
  }
  if (info->bool_infile) {
    printf("infile: %s\n", info->in_file);
  }
  else {
    //printf("no input redirection.\n");
  }
  if (info->bool_outfile) {
    printf("outfile: %s\n", info->out_file);
  }
  else {
    //printf("no output redirection.\n");
  }
  if (info->bool_background) {
    printf("Background process.\n");
  } 
  else { 
    //printf("Foreground process.\n");
  }
}
 
void free_info (parse_info *info) {
  int i,j;
  struct command_type *comm;

  if (NULL == info) return;
  for (i = 0; i < PIPE_MAX_NUM; i++) {
    comm = &(info->com_array[i]);
    for (j = 0; j<comm->var_num; j++) {
      free(comm->var_list[j]);
    }
    if (NULL != comm->command) {
      free(comm->command);
    }
  }
  free(info);
}
