#include <iostream>
#include <string>
#include <sys/dir.h>
#include <fstream>
#include <sstream>
#include <list>

struct _mem_read {
  std::string path;
  std::string start;
  std::string end;
  std::string offset;
  std::string perms;
  unsigned int size;
  unsigned int p_clean;
  unsigned int p_dirty;
  unsigned int s_clean;
  unsigned int s_dirty;
} typedef mem_read;

void free_list(std::list<mem_read *> wish90);
void print_list(std::list<mem_read> arteam);
std::list<mem_read> mem_map(int pid);