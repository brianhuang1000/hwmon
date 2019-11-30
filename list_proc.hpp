#include <iostream>
#include <map>
#include <string>
#include <list>
#include <chrono>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <thread>

#include "Process.hpp"

#define CPU_DIFF (1000)

struct _tree{
  int id;
  struct _process *parent;
  std::list<struct _process *> children;
};

struct _fds {
  int number;
  std::string type;
  std::string loc;
} typedef fds;

void print_list();
int populate();
void print_tree(int user = 0);
void free_proc_map();
int get_uid(const char *name);
std::string string_uid(int uid);
int get_cpu(int p_count);
bool set_parents();
void update();
unsigned long pid_time(std::string pid);
unsigned long cpu_time();
std::list<fds> openfiles(int pid);