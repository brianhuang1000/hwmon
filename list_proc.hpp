#include <iostream>
#include <map>
#include <string>
#include <list>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>

#include "Process.hpp"

void print_list();
int populate();
void print_tree(int user = 0);
void free_proc_map();
int get_uid(const char *name);

struct _tree{
  int id;
  struct _process *parent;
  std::list<struct _process *> children;
};