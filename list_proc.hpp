#ifndef LIST_PROC_HPP
#define LIST_PROC_HPP

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <chrono>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>

#include <sys/sysinfo.h>

#include "Process.hpp"

void print_list();
int populate();
void print_tree(int user = 0);
void free_proc_map();
int get_uid(const char *name);
std::string string_uid(int uid);
int get_cpu(int p_count);
bool set_parents();
void update();

struct _tree{
  int id;
  struct _process *parent;
  std::list<struct _process *> children;
};

#endif // LIST_PROC_HPP
