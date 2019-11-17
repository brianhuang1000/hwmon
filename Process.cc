#include "Process.hpp"

void Process::print_info() {
  printf("id: %d, name: %s, state: %c, ppid: %d, uid: %d, vmrss: %lu\n", pid, name.c_str(), state, ppid, uid, vmrss);
}

void Process::create(int id) {
  this->pid = id;
  pop_core();
  //print_info();
}

// void Process::set_parent(Process *parent) {
//   this->parent = parent;
// }

void Process::add_child(Process *child) {
  //std::cout << this->name << " pushed back " << child->name << std::endl;
  this->children.push_back(child);
}

void Process::print_children(int tabs) {
  for (auto it = this->children.begin(); it != this->children.end(); it++) {
    for (int i = 0; i < tabs; i++) {
      std::cout << " ";
    }
    std::cout << (*it)->name << std::endl;
    (*(*it)).print_children(tabs+1);
  }
}

bool Process::pop_core() {
  int got = 0;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(this->pid) + "/status");
  if (infile.is_open()){
    std::string line;
    while (std::getline(infile, line) && got < 5) {
      std::istringstream is_line(line);
      std::string key;
      std::string value;
      is_line >> key;
      is_line >> value;
      key.pop_back();
      if (!key.compare("VmRSS")) {
        this->vmrss = std::stoul(value);
        got++;
      }
      else if (!key.compare("Name")) {
        this->name = value;
        got++;
      }
      else if (!key.compare("State")) {
        this->state = value.front();
        got++;
      }
      else if (!key.compare("PPid")) {
        this->ppid = stoi(value);
        got++;
      }
      else if (!key.compare("UID")) {
        this->uid = stoi(value);
        got++;
      }
    }
  }
  else {
    return false;
  }
  return true;
}