#include "list_proc.hpp"

//testing commit

std::list<Process *> g_parents;
std::map<int, Process> g_proc_map; 

int main(int argc, const char** argv) {
  populate(1000);
  print_tree();
  free_proc_map();
}

int populate(int user) {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("/proc")) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string name(ent->d_name);
      if(name.find_first_not_of("0123456789") == std::string::npos) {
        int curid = stoi(name);
        g_proc_map[curid].create(stoi(name));
        if (g_proc_map[curid].ppid == 0) {
          g_parents.push_back(&g_proc_map[stoi(name)]);
        }
        else {
          if (g_proc_map.find(g_proc_map[curid].ppid) == g_proc_map.end()){
            std::cout << "NO ENTRY" << std::endl;
          }
          else {
            g_proc_map[g_proc_map[curid].ppid].add_child(&(g_proc_map[curid]));
          }
        }
      }
    }
    closedir (dir);
  } else {
    perror ("");
    return -1;
  }
  return 1;
}

void free_proc_map() {
  for (auto it = g_proc_map.cbegin(), next_it = it; it != g_proc_map.cend(); it = next_it) {
    //free(it->second);
    ++next_it;
    g_proc_map.erase(it);
  }
}



void print_tree() {
  for (auto it = g_parents.begin(); it != g_parents.end(); it++) {
    //Print tree;
    std::cout << (*it)->name << std::endl;
    // for (auto cld = (*it)->children.begin(); cld != (*it)->children.begin(); cld++) {
    //   std::cout << "inside\t" << (*cld)->name << std::endl;
    // }
    (*(*it)).print_children(1);
  }
}

void print_list() {
  std::list<Process *>::iterator it;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
    std::printf("id: %d\n", (*it)->pid);
  }
}