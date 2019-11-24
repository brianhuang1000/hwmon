#ifndef PROCESS_HPP
#define PROCESS_HPP

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
    unsigned long swap;
    std::string name;
    char state;
    int ppid;
    int uid;
    float cpu;
    void print_info();
    void create(int);
    Process *parent;
    std::list<Process *> children;
    void print_children(int tabs, int user = 0);
    //void set_parent(Process *parent);
    void add_child(Process *child);
    std::string get_name();
};

#endif // PROCESS_HPP
