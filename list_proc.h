#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <dirent.h>

typedef std::map<std::string, std::string> p_info;
p_info test_proc(std::string pid);
void print_list();
void print_info(p_info info);

struct _process{
  unsigned int id;
  p_info info;
  struct _process *parent;
  std::list<struct _process *> children;
};
typedef _process process;