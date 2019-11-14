#include "list_proc.h"

std::list<process> g_parents; 

int main(int argc, const char** argv) {
  process test;
  test.id = 12;
  test.info = test_proc("12");
  g_parents.push_back(test);
  print_list();
}

void blop() {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("/proc")) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string name(ent->d_name);
      if(name.find_first_not_of("0123456789") == std::string::npos) {
        std::ifstream infile;
        infile.open("/proc/" + name + "/status");
        if (infile.is_open()){
          p_info values;
          std::string line;
          while (std::getline(infile, line)) {
            std::istringstream is_line(line);
            std::string key;
            std::string value;
            is_line >> key;
            is_line >> value;
          }
        }
        std::string part;
        std::string value;
        infile >> part;
        infile >> value;
        printf("%s, %s\n", part.c_str(), value.c_str());
      }
    }
    closedir (dir);
  } else {
    perror ("");
    return;
  }
  std::cout << "yes\n";
}

p_info test_proc(std::string pid) {
  std::ifstream infile;
  infile.open("/proc/" + pid + "/status");
  if (infile.is_open()){
    p_info values;
    std::string line;
    while (std::getline(infile, line)) {
      std::istringstream is_line(line);
      std::string key;
      std::string value;
      is_line >> key;
      is_line >> value;
      key.pop_back();
      values[key] = value;
    }
    print_info(values);
    return values;
  }
  else {
    p_info bad;
    return bad;
  } 
}

void print_list() {
  std::list<process>::iterator it;
  for (it = g_parents.begin(); it != g_parents.end(); it++) {
    std::printf("id: %d, #children: %d\n", it->id, (unsigned int) it->children.size());
  }
}

void print_info(p_info info) {
  p_info::iterator it;
  for (it = info.begin(); it != info.end(); it++) {
    std::printf("%s:%s\n", it->first.c_str(), it->second.c_str());
  } 
}