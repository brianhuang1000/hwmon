#include "list_proc.hpp"

std::list<Process *> g_parents;
std::map<int, Process *> g_proc_map;

/*
 * Testing main function
 */

// int main(int argc, const char** argv) {
//   //openfiles(3157);
//   if (argc != 2){
//     std::cout << "usage: ./proc username\n";
//     return -1;
//   }
//   int userid = get_uid(argv[1]);
//   populate();
//   set_parents();
//   get_cpu(get_nprocs_conf());
//   update();
//   update();
//   update();
//   print_tree(userid);
//   free_proc_map();
// }

/*
 * Populates g_proc_map with directories of processes and g_parents list
 */

int populate() {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("/proc")) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string name(ent->d_name);
      if (name.find_first_not_of("0123456789") == std::string::npos) {
        int curid = stoi(name);
        Process *temp = new Process(curid);
        g_proc_map.insert({curid, temp});
        if (temp->ppid == 0) {
          g_parents.push_back(temp);
        }
      }
    }
    closedir (dir);
  } else {
    perror ("Error opening proc folder\n");
    return -1;
  }
  return 1;
}

/*
 * Builds children tree
 */

bool set_parents() {
  for (auto it = g_proc_map.begin(); it != g_proc_map.end(); it++) {
    if (it->second->ppid != 0) {
      if (g_proc_map.count(it->second->ppid) == 0) {
        std::cout << "ERROR: " << it->second->ppid << " NOT FOUND\n";
      } else {
        g_proc_map[it->second->ppid]->add_child((it->second));
      }
    }
  }
  return true;
}

/*
 * Frees/clears g_proc_map and g_parents
 */

void free_proc_map() {
  for (auto it = g_proc_map.cbegin(), next_it = it; it != g_proc_map.cend(); it = next_it) {
    ++next_it;
    delete(it->second);
    g_proc_map.erase(it);
  }
  g_parents.clear();
}

/*
 * Prints tree for display or debugging
 */

void print_tree(int user) {
  for (auto it = g_parents.begin(); it != g_parents.end(); it++) {
    int big_pp = 0;
    if((*it)->uid == user || user == 0){
      big_pp++;
      std::cout << (*it)->name << "\t" << (*it)->state << "\t" << (*it)->pid;
      std::cout << "\t" << ((float)((*it)->vmrss + (*it)->swap) / 1000);
      std::cout << "\tcpu: " << (*it)->cpu_precent << std::endl;
    }
    (*(*it)).print_children(big_pp, user);
  }
}

/*
 * Prints parents list
 */

void print_list() {
  std::list<Process *>::iterator it;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
    std::printf("id: %d\n", (*it)->pid);
  }
}

/*
 * Translates name to uid number
 */

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
  } else {
    std::cout << "Error opening passwd\n";
  }
  return -1;
}

/*
 * Translates uid number toname
 */

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
  } else {
    std::cout << "Error opening passwd\n";
  }
  return "error";
}

/*
 * Returns cpu jiffies
 */

unsigned long cpu_time() {
  FILE *f = fopen("/proc/stat", "r");
  unsigned long total = 0;
  if (f != NULL) {
    unsigned long cpus[10];
    int number = fscanf(f,"%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            &cpus[0], &cpus[1], &cpus[2], &cpus[3], &cpus[4], &cpus[5],
            &cpus[6], &cpus[7], &cpus[8], &cpus[9]);
    if (number != 10) {
      std::cout << "Error: issue opening /proc/stat\n";
      return 100;
    }
    for (int i = 0; i < 10; i++) {
      //std::cout << cpus[i] << " ";
      total += cpus[i];
    }
    //std::cout << std::endl;
    //std::cout << "global cpu: " << total << std::endl;
    fclose(f);
  } else {
    std::cout << "Error getting cpu jiffies\n";
  }
  return total;
}

/*
 * Returns jiffies of a process
 */

unsigned long pid_time(std::string pid) {
  FILE *f = fopen(("/proc/" + pid + "/stat").c_str(), "r");
  if (f != NULL) {
    unsigned long utime = 0;
    unsigned long stime = 0;
    unsigned long cutime = 0;
    unsigned long cstime = 0;
    char junk[255] = " ";
    while (strstr(junk,")") == NULL) {
      if(fscanf(f,"%[^ ]%*c",junk) ==  EOF){
        break;
      }
    }
    int number = fscanf(f,"%*c %*d %*d %*d %*d %*d %*d %*d "
            "%*d %*d %*d %lu %lu %lu %lu", &utime, &stime, &cutime, &cstime);
    if (number != 4) {
      std::cout << "Unable to read " << pid << " cpu usage\n";
    }
    return utime + stime + cutime + cstime;
  } else {
    std::cout << "Error getting process cpu jiffies\n";
  }
  return 0;
}

/*
 * Populates the cpu % of every process
 */

int get_cpu(int p_count) {
  for (auto it = g_proc_map.begin(); it != g_proc_map.end(); it++) {
    it->second->cpu_begin = pid_time(std::to_string(it->second->pid));
  }
  unsigned long cpu_start = cpu_time();
  std::this_thread::sleep_for(std::chrono::milliseconds(CPU_DIFF));
  unsigned long cpu_end = cpu_time();
  for (auto it = g_proc_map.begin(); it != g_proc_map.end(); it++) {
    it->second->cpu_end = pid_time(std::to_string(it->second->pid));
  }
  unsigned long cpu_diff = cpu_end - cpu_start;
  for (auto it = g_proc_map.begin(); it != g_proc_map.end(); it++) {
    it->second->cpu_precent = ((float)100 *
      (it->second->cpu_end - it->second->cpu_begin) / cpu_diff) * p_count;
  }
  return 0;
}

/*
 * Removes process and children
 */

void remove(int pid) {
  g_proc_map[g_proc_map[pid]->ppid]->children.remove(g_proc_map[pid]);
  for (auto it = g_proc_map[pid]->children.begin(), next_it = it;
        it != g_proc_map[pid]->children.end(); it = next_it) {
    next_it++;
    remove((*it)->pid);
  }
  delete(g_proc_map[pid]);
  g_proc_map.erase(g_proc_map.find(pid));
}

/*
 * Returns list of fds of open files by a pid
 */

std::list<fds> openfiles(int pid) {
  DIR *dir;
  struct dirent *ent;
  std::list<fds> out;
  if ((dir = opendir (("/proc/" + std::to_string(pid) + "/fd").c_str()))
        != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string name(ent->d_name);
      if(name.find_first_not_of("0123456789") == std::string::npos) {
        fds temp;
        temp.number = std::stoi(name);
        char dets[64] = "";
        int rlret = readlink(("/proc/" + std::to_string(pid) + "/fd/" + name).c_str(),
                  dets, 64);
        if (rlret == -1) {
          continue;
        }
        if (strstr(dets, ":[") != NULL){
          char *blep = strtok(dets, ":[");
          char *bloop = strtok(NULL, "");
          std::string type(blep);
          std::string loc(bloop);
          temp.type = type;
          temp.loc = loc;
        }
        else {
          temp.type = "file";
          temp.loc = dets;
        }
        out.push_back(temp);
        // std::cout << number << ", " << temp.type << ", ";
        // std::cout << temp.loc << std::endl;
      }
    }
    closedir (dir);
  } else {
    std::cout << "error in opening fd directory\n";
    return out;
  }
  return out;
}

/*
 * Updates g_proc_map
 */

void update() {
  free_proc_map();
  populate();
  set_parents();
  get_cpu(get_nprocs_conf());
}
