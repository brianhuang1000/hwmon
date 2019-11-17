#include <iostream>
#include <map>
#include <string>
#include <list>
#include <dirent.h>
#include<sys/dir.h>

#include "Process.hpp"

void print_list();
int populate(int user = 0);
void print_tree();
void free_proc_map();

struct _tree{
  int id;
  struct _process *parent;
  std::list<struct _process *> children;
};