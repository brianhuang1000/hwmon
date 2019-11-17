#include <map>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Process {
  private:
    bool pop_core();
  public:
    int pid;
    unsigned long vmrss;
    std::string name;
    char state;
    int ppid;
    int uid;
    void print_info();
    void create(int);
    Process *parent;
    std::list<Process *> children;
    void print_children(int tabs);
    //void set_parent(Process *parent);
    void add_child(Process *child);
};