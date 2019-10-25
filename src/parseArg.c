#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static void init_parse_arg(void);
static int is_able(void);
static struct ArgStruct args;

/* static struct ArgsExtraFlag */
/* { */

/* }; */

int parse_argc(int argc, char ** argv)
{
  init_parse_arg();
  for(int i = 0; i < argc; i++) {
    if(!strncmp(argv[i], "-?", strlen("-?")) || !strncmp(argv[i], "--help", strlen("--help"))){
      printf("WOW\n");
      return 1;
    }
  }
  return is_able();
}


void init_parse_arg(void)
{
  args.from.port = "8000";
  args.from.addr = NULL;
  args.to.port = NULL;
  args.to.addr = NULL;
  args.type = UNDEFINED;
}

struct ArgStruct * get_args(void)
{
  return &args;
}

int is_able(void)
{
  if(args.type == UNDEFINED){
    return -1;
  }
  if(!args.from.addr){
    return -1;
  }
  if(!args.to.addr){
    return -1;
  }
  if(!args.to.port){
    return -1;
  }
  return 0;
}
