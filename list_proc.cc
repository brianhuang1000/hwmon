#include "list_proc.h"

typedef struct {
  std::string name;
  unsigned int id;
} process;

int main(int argc, const char** argv) {
  test_proc("12");
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
      std::cout << key << " " << value << "\n";
    }
    return values;
  }
  else {
    p_info bad;
    return bad;
  }
}