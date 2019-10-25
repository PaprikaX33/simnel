#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static void init_parse_arg(void);
static int is_able(void);
static int parse_flag(char const *);
static void print_help(void);
static void print_invalid(void);
static void print_version(void);
static void print_unrecog(char const *);
static char const * snip_dir(char const *);

static char const * prnam;

static struct ArgStruct args;

int parse_argc(int argc, char ** argv)
{
  init_parse_arg();
  prnam = snip_dir(argv[0]);
  for(int i = 1; i < argc; i++) {
    if(*(argv[i]) == '-'){
      int const flg_res = parse_flag(argv[i]);
      if(flg_res){
        return flg_res;
      }
    }
  }
  if(is_able()){
    print_invalid();
    return -1;
  }
  return 0;
}

int parse_flag(char const * flg)
{
  if(!strncmp(flg, "-?", strlen("-?")) || !strncmp(flg, "--help", strlen("--help"))){
    print_help();
    return 1;
  }
  if(!strncmp(flg, "-V", strlen("-V")) || !strncmp(flg, "--version", strlen("--version"))){
    print_version();
    return 1;
  }
  print_unrecog(flg);
  return -1;
}

char const * snip_dir(char const * in)
{
  char const * found = in;
  while(*in != '\0'){
    if(*in == '/'){
      found = in+1;
    }
    in++;
  }
  return found;
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

void print_version(void)
{
  printf("%s v%d.%d.%d\n"
         "simnel suite, a TCP tunnling suite.\n\n"
         "There is NO WARRANTY, to the extent permitted by law.\n"
         , prnam, VER_MAJOR, VER_MINOR, VER_PATCH);
}

void print_invalid(void)
{
  fprintf(stderr,
          "%s: Incomplete information given!!\n"
          "%s: Try \'%s --help\' for more information\n",
          prnam, prnam, prnam);
}

void print_unrecog(char const * flg)
{
  fprintf(stderr,
          "%s: Unknown flag \"%s\"\n"
          "%s: Try \'%s --help\' for more information\n",
          prnam, flg, prnam, prnam);
}

void print_help(void)
{
  printf("Usage: %s [OPTION] <SOURCE ADDR>:<SOURCE PORT> <DEST ADDR>:<DEST PORT>\n", prnam);
  printf("Open a listening connection on the SOURCE, and forward the data stream to DEST.\n\n");
  //printf("");
}
