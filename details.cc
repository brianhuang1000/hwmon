#include "details.hpp"

std::list<mem_read> mem_map(int pid) {
  std::list<mem_read> ret;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(pid) + "/smaps");
  if (infile.is_open()) {
    while (!infile.eof()) {
      mem_read temp;
      std::string title;
      std::getline(infile, title);
      temp.start = title.substr(0, title.find("-"));
      title.erase(0, title.find("-") + 1);
      std::istringstream is_line(title);
      is_line >> temp.end;
      is_line >> temp.perms;
      is_line >> temp.offset;
      std::string junk;
      is_line >> junk;
      is_line >> junk;
      is_line >> temp.path;
      bool next_part = true;
      while (next_part) {
        std::string line;
        std::getline(infile, line);
        std::string key;
        std::string value;
        std::istringstream rval(line);
        rval >> key;
        rval >> value;
        key.pop_back();
        if (key.compare("VmFlags") == 0) {
          std::getline(infile, line);
          next_part = false;
        }
        if (!key.compare("Size")) {
          temp.size = std::stoi(value, 0);
        }
        else if (!key.compare("Shared_Clean")) {
          temp.s_clean = std::stoi(value, 0);
        }
        else if (!key.compare("Shared_Dirty")) {
          temp.s_dirty = std::stoi(value, 0);
        }
        else if (!key.compare("Private_Clean")) {
          temp.p_clean = std::stoi(value, 0);
        }
        else if (!key.compare("Private_Dirty")) {
          temp.p_dirty = std::stoi(value, 0);
        }
      }
      ret.push_back(temp);
    }
  }
  else {
    std::cout << "does not work\n";
  }
  return ret;
}

void print_list(std::list<mem_read> arteam) {
  int number = 0;
  for (auto it = wish90.begin(); it!= wish90.end(); it++) {
    if (it->path == "") {
      std::cout << "n/a";
    }
    std::cout << it->path << ", " << it->start << ", " << it->end;
    std::cout << ", " << it->perms << ", " << it->offset << ", " << it->size << "kB, ";
    std::cout << it->s_clean + it->s_dirty << ", " << it->p_clean + it->p_dirty << std::endl;
    number ++;
  }
  std::cout << "lines: " << number << std::endl;
}

void free_list(std::list<mem_read *> wish90) {
  for (auto it = wish90.cbegin(), next_it = it; it != wish90.cend(); it = next_it) {
    ++next_it;
    free(*it);
    wish90.erase(it);
  }
}

int main(int argc, const char** argv) {
  std::list<mem_read> wish90 = mem_map(9896);
  print_list(wish90);
  //free_list(wish90);
}