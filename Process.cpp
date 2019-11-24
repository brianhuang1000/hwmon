#include "Process.hpp"

void Process::print_info() {
  printf("id: %d, name: %s, state: %c, ppid: %d, uid: %d, vmrss: %lu, swap: %lu\n", pid, name.c_str(), state, ppid, uid, vmrss, swap);
}

void Process::create(int id) {
  this->pid = id;
  pop_core();
  //print_info();
}

std::string Process::get_name(){
    return "this->name";
}

void Process::add_child(Process *child) {
  this->children.push_back(child);
}

bool Process::pop_core() {
  int got = 0;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(this->pid) + "/status");
  if (infile.is_open()){
    std::string line;
    while (std::getline(infile, line) && got < 6) {
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
      else if (!key.compare("Uid")) {
        this->uid = stoi(value);
        got++;
      }
      else if (!key.compare("VmSwap")) {
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
