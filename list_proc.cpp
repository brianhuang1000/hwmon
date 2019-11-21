#include "list_proc.hpp"
#include "systemmonitor.cpp"

// TODO: safe opening errors

std::list<Process *> g_parents;
std::map<int, Process> g_proc_map;

int populate() {
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
        if (g_proc_map[curid].ppid != 0 && g_proc_map.find(g_proc_map[curid].ppid) == g_proc_map.end()){
          std::cout << "NO ENTRY " << g_proc_map[curid].name << "parent: " << g_proc_map[curid].ppid << std::endl;
        }
        g_proc_map[g_proc_map[curid].ppid].add_child(&(g_proc_map[curid]));
      }
    }
    closedir (dir);
  } else {
    perror ("i dont like this very much\n");
    return -1;
  }
  return 1;
}

void free_proc_map() {
  for (auto it = g_proc_map.cbegin(), next_it = it; it != g_proc_map.cend(); it = next_it) {
    ++next_it;
    g_proc_map.erase(it);
  }
}

void print_tree(int user) {
  for (auto it = g_parents.begin(); it != g_parents.end(); it++) {
    int big_pp = 0;
    if((*it)->uid == user || user == 0){
      big_pp++;
      std::cout << (*it)->name << "\t" << (*it)->state << "\t" << (*it)->pid << "\t" << ((float)((*it)->vmrss + (*it)->swap) / 1000) << std::endl;
    }
    (*(*it)).print_children(big_pp, user);
  }
}

void print_list() {
  std::list<Process *>::iterator it;
  process proc;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
      (*it)->cpu = 62;
     add_process(NULL, (*it));
  }
}

int get_uid(const char *name) {
  FILE *f = fopen("/etc/passwd", "r");
  char username[62];
  int retname;
  if (f != NULL){
    while (fscanf(f, "%[^:]:x:%d:%*[^\n]\n", username, &retname) != EOF) {
      if (!strcmp(username, name)) {
        return retname;
      }
    }
  }
  return -1;
}

std::string string_uid(int uid) {
  FILE *f = fopen("/etc/passwd", "r");
  char username[62];
  int retname;
  if (f != NULL){
    while (fscanf(f, "%[^:]:x:%d:%*[^\n]\n", username, &retname) != EOF) {
      if (retname == uid) {
        return std::string(username);
      }
    }
  }
  return "error";
}
