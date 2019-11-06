#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

#define COMPARE_FLAG(flg, shrt, lng) (!strncmp(flg, shrt, strlen(shrt)) || !strncmp(flg, lng, strlen(lng)))

enum FlagParserState {
                      ERROR,
                      CONTINUE,
                      EXIT,
                      IPORT
};

static void init_parse_arg(void);
static int is_able(void);
static enum FlagParserState parse_flag(char const *);
static void print_help(void);
static void print_invalid(void);
static void print_unrecog_arg(char const *);
static void print_no_args(char const *);
static void print_conflict(char const *, char const *);
static int is_NUM(char const *);
static void print_version(void);
static void print_unrecog(char const *);
static char const * snip_dir(char const *);
static void init_bypass(void);

static char const * prnam;

static struct ArgStruct args;

int parse_argc(int argc, char ** argv)
{
  init_parse_arg();
  prnam = snip_dir(argv[0]);
  for(int i = 1; i < argc; i++) {
    if(*(argv[i]) == '-'){
      int const flg_res = parse_flag(argv[i]);
      switch(flg_res) {
      case ERROR:
        return -1;
      case EXIT:
        return 1;
      case CONTINUE:
      default:
        break;
      case IPORT:
        {
          char const * const flag = argv[i];
          if(++i == argc) {
            print_no_args(flag);
            return -1;
          }
          char const * const arg = argv[i];
          if(!is_NUM(arg)){
            print_unrecog_arg(arg);
            return -1;
          }
          args.from.port = arg;
        }
        break;
      }
    }
  }
  init_bypass();
  if(is_able()){
    print_invalid();
    return -1;
  }
  return 0;
}

enum FlagParserState parse_flag(char const * flg)
{
  if(COMPARE_FLAG(flg, "-P", "--version")){
    return IPORT;
  }
  if(COMPARE_FLAG(flg,"-q", "--quiet")){
    if(args.verb == VERBOSE) {
      print_conflict(flg, "-v/--verbose");
      return ERROR;
    }
    args.verb = QUIET;
    return CONTINUE;
  }
  if(COMPARE_FLAG(flg,"-v", "--verbose")){
    if(args.verb == QUIET) {
      print_conflict(flg, "-q/--quiet");
      return ERROR;
    }
    args.verb = VERBOSE;
    return CONTINUE;
  }
  if(COMPARE_FLAG(flg, "-?", "--help")){
    print_help();
    return EXIT;
  }
  if(COMPARE_FLAG(flg, "-V", "--version")){
    print_version();
    return EXIT;
  }
  print_unrecog(flg);
  return ERROR;
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
  args.verb = NORMAL;
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

void init_bypass(void)
{
  //args.from.port = "8000";
  args.from.addr = "0";
  args.to.port = "duckduckgo.com";
  args.to.addr = "80";
  args.type = BYPASS;
}

int is_NUM(char const * txt)
{
  while(*txt != '\0'){
    if(isdigit(*txt)){
      txt++;
    }
    else {
      return 0;
    }
  }
  return 1;
}

void print_conflict(char const * flag, char const * others)
{
  fprintf(stderr,
          "%s: Conflicting flag of\'%s\' with %s\n",
          prnam, flag, others);
  fprintf(stderr,
          "%s: Try \'%s --help\' for more information\n",
          prnam, prnam);
}

void print_no_args(char const * flg)
{
  fprintf(stderr,
          "%s: Flag \'%s\' requires a [NUM] argument\n"
          "%s: Try \'%s --help\' for more information\n",
          prnam, flg, prnam, prnam);
}

void print_version(void)
{
  printf("%s v%d.%d.%d\n"
         "simnel suite, a TCP tunnling suite.\n\n"
         "There is NO WARRANTY, to the extent permitted by law.\n",
         prnam, VER_MAJOR, VER_MINOR, VER_PATCH);
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

void print_unrecog_arg(char const * arg)
{
  fprintf(stderr,
          "%s: Unknown argument \"%s\"\n"
          "%s: Try \'%s --help\' for more information\n",
          prnam, arg, prnam, prnam);
}

void print_help(void)
{
  printf("Usage: %s [OPTION] <ADDR>:<PORT>\n", prnam);
  printf("Open a listening connection on localhost (default port: 8000),\n");
  printf("and forward the data stream to ADDR (default port: 80).\n\n");
  printf("  -P,\t--port\t[NUM]\tPort number for the listening port\n");
  printf("  -q,\t--quiet\t\tRun in quiet mode, reports error only\n");
  printf("  -v,\t--verbose\tRun in verbose mode, reports everything\n");
  printf("  -?,\t--help\t\tPrint this help text\n");
  printf("  -V,\t--version\tPrint the version information\n");
}
