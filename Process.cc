#include "Process.hpp"

/*
 * Prints contenst
 */

void Process::print_info() {
  printf("id: %d, name: %s, state: %c, ppid: %d, uid:"
  " %d, vmrss: %lu, swap: %lu\n",
  pid, name.c_str(), state, ppid, uid, vmrss, swap);
}

/*
 * Init
 */
Process::Process(int id) {
  this->pid = id;
  p_init();
  pop_core();
}

/*
 * Adds child to children
 */

void Process::add_child(Process *child) {
  this->children.push_back(child);
}

/*
 * Recursive tree print
 */

void Process::print_children(int tabs, int user) {
  for (auto it = this->children.begin(); it != this->children.end(); it++) {
    if (((*it)->uid == user) || (user == 0)) {
      for (int i = 0; i < tabs; i++) {
        std::cout << "-";
      }
      std::cout << "|" << (*it)->name << "\t" << (*it)->state << "\t";
      std::cout << (*it)->pid << "\t" << std::flush;
      std::cout << ((float)((*it)->vmrss + (*it)->swap) / 1000) << "\tcpu: ";
      std::cout << (*it)->cpu_precent << std::endl;
      (*(*it)).print_children(tabs + 1, user);
    }
    else {
      (*(*it)).print_children(tabs, user);
    }
  }
}

/*
 * Populates details
 */

bool Process::pop_core() {
  int got = 0;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(this->pid) + "/status");
  if (infile.is_open()) {
    std::string line;
    while ((std::getline(infile, line)) && (got < 6)) {
      std::istringstream is_line(line);
      std::string key;
      std::string value;
      is_line >> key;
      is_line >> value;
      key.pop_back();
      if (!key.compare("VmRSS")) {
        this->vmrss = std::stoul(value, 0);
        got++;
      } else if (!key.compare("Name")) {
        this->name = value;
        got++;
      } else if (!key.compare("State")) {
        this->state = value.front();
        got++;
      } else if (!key.compare("PPid")) {
        this->ppid = stoi(value);
        got++;
      } else if (!key.compare("Uid")) {
        this->uid = stoi(value);
        got++;
      } else if (!key.compare("VmSwap")) {
        this->swap = stoi(value);
        got++;
      }
    }
  }
  else {
    std::cout << ":tirpitzthink:" << std::endl;
    return false;
  }
  return true;
}

/*
 * Updates core
 */

bool Process::update() {
  int got = 0;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(this->pid) + "/status");
  if (infile.is_open()) {
    std::string line;
    while (std::getline(infile, line) && got < 3) {
      std::istringstream is_line(line);
      std::string key;
      std::string value;
      is_line >> key;
      is_line >> value;
      key.pop_back();
      if (!key.compare("VmRSS")) {
        this->vmrss = std::stoul(value, 0);
        got++;
      } else if (!key.compare("State")) {
        this->state = value.front();
        got++;
      } else if (!key.compare("VmSwap")) {
        this->swap = stoi(value);
        got++;
      }
    }
    return false;
  }
  else {
    return true;
  }
}

/*
 * Sets everything to 0
 */

void Process::p_init() {
  cpu_begin = 0;
  cpu_end = 0;
  cpu_precent = 0;
  vmrss = 0;
  swap = 0;
  name = "err";
  state = 'e';
  ppid = 0;
  uid = 0;
}