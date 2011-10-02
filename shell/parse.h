#define MAX_VAR_NUM 11
#define PIPE_MAX_NUM 11
#define FILE_MAX_SIZE 41

struct command_type {
  char *command;
  char *var_list[MAX_VAR_NUM];
  int var_num;
};

/* parsing information structure */
typedef struct {
  int   bool_infile;		       /* boolean value - infile specified */
  int   bool_outfile;		       /* boolean value - outfile specified */
  int   bool_background;		       /* run the process in the background? */
  struct command_type com_array[PIPE_MAX_NUM];
  int   pipe_num;
  char  in_file[FILE_MAX_SIZE];	       /* file to be piped from */
  char  out_file[FILE_MAX_SIZE];	       /* file to be piped into */
} parse_info;

/* the function prototypes */
parse_info *parse(char *);
void free_info(parse_info *);
void print_info(parse_info *);











