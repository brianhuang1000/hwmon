#include <map>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Process {
  private:
    bool pop_core();
    void p_init();
  public:
    int pid;
    int cpu_begin;
    int cpu_end;
    float cpu_precent;
    unsigned long vmrss;
    unsigned long swap;
    std::string name;
    char state;
    int ppid;
    int uid;
    void print_info();
    Process(int);
    Process *parent;
    std::list<Process *> children;
    void print_children(int tabs, int user = 0);
    //void set_parent(Process *parent);
    void add_child(Process *child);
    bool update();
};