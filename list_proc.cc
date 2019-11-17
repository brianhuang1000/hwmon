#include "list_proc.hpp"

// TODO: safe opening errors

std::list<Process *> g_parents;
std::map<int, Process> g_proc_map; 

int main(int argc, const char** argv) {
  if (argc != 2){
    std::cout << "usage: ./proc username\n";
    return -1;
  }
  int userid = get_uid(argv[1]);
  std::cout << "uid for itsmiki: " << userid << std::endl;
  populate();
  print_tree(userid);
  free_proc_map();
}

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
    //free(it->second);
    ++next_it;
    g_proc_map.erase(it);
  }
}

void print_tree(int user) {
  for (auto it = g_parents.begin(); it != g_parents.end(); it++) {
    //Print tree;
    int big_pp = 0;
    if((*it)->uid == user || user == 0){
      big_pp++;
      std::cout << (*it)->name << std::endl; 
    }
    // for (auto cld = (*it)->children.begin(); cld != (*it)->children.begin(); cld++) {
    //   std::cout << "inside\t" << (*cld)->name << std::endl;
    // }
    //std::cout << "ok\n";
    (*(*it)).print_children(big_pp, user);
  }
}

void print_list() {
  std::list<Process *>::iterator it;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
    std::printf("id: %d\n", (*it)->pid);
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