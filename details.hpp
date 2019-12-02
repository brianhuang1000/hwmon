#ifndef DETAILS_HPP
#define DETAILS_HPP

#include <iostream>
#include <string>
#include <sys/dir.h>
#include <bits/stdc++.h> 
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <list>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>
#include <stdio.h>

struct _mem_read {
  std::string path;
  std::string start;
  std::string end;
  std::string offset;
  std::string perms;
  unsigned int size;
  unsigned int p_clean;
  unsigned int p_dirty;
  unsigned int s_clean;
  unsigned int s_dirty;
} typedef mem_read;


//use kibs
struct _proc_prop {
  std::string name; //name
  std::string user; //uid + user
  std::string status; //state
  unsigned long memory; //rss + swap
  unsigned long v_mem; //vm size
  unsigned long res_mem; //rss
  unsigned long sh_mem; //rss_file + rssshmem
  float cpu; //compute cpu
  std::string uptime; //jiffies / sysconf(_SC_CLK_TCK) 
  std::string started; //(/status/starttime / sysconf(_SC_CLK_TCK)) + boottime) to date
  int nice;
  int id;
}  typedef proc_prop;

void free_list(std::list<mem_read *> wish90);
void print_list(std::list<mem_read> arteam);
std::list<mem_read> mem_map(int pid);

void printdetails(proc_prop details);

#endif // DETAILS_HPP
