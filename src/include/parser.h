#ifndef PARSER_SIMNEL_HEAD_HPP
#define PARSER_SIMNEL_HEAD_HPP

struct ArgStruct
{
  struct {
    char const * addr;
    char const * port;
  } from, to;
  enum ArgType {
                UNDEFINED,
                SERVER,
                CLIENT
  } type;
};

int parse_argc(int argc, char ** argv);
struct ArgStruct * get_args(void);

#endif //PARSER_SIMNEL_HEAD_HPP
