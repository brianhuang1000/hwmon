#include "details.hpp"
#include "list_proc.hpp"

/*
 * Returns a list of mem_read of the memory map (read from smaps) of the process
 */

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
        } else if (!key.compare("Shared_Clean")) {
          temp.s_clean = std::stoi(value, 0);
        } else if (!key.compare("Shared_Dirty")) {
          temp.s_dirty = std::stoi(value, 0);
        } else if (!key.compare("Private_Clean")) {
          temp.p_clean = std::stoi(value, 0);
        } else if (!key.compare("Private_Dirty")) {
          temp.p_dirty = std::stoi(value, 0);
        }
      }
      ret.push_back(temp);
    }
  } else {
    std::cout << "Unable to open file for memory map\n";
  }
  infile.close();
  return ret;
}

/*
 * Prints mem_read list for debugging
 */

void print_list(std::list<mem_read> arteam) {
  int number = 0;
  for (auto it = arteam.begin(); it!= arteam.end(); it++) {
    if (it->path == "") {
      std::cout << "n/a";
    }
    std::cout << it->path << ", " << it->start << ", " << it->end << ", ";
    std::cout << it->perms << ", " << it->offset << ", " << it->size << "kB, ";
    std::cout << it->s_clean + it->s_dirty << ", " << it->p_clean + it->p_dirty;
    std::cout << std::endl;
    number++;
  }
  std::cout << "lines: " << number << std::endl;
}

/*
 * Frees mem_read list
 */

void free_list(std::list<mem_read *> wish90) {
  for (auto it = wish90.cbegin(), next_it = it;
        it != wish90.cend(); it = next_it) {
    ++next_it;
    free(*it);
    wish90.erase(it);
  }
}

/*
 * Converts jiffies to HH:MM:SS
 */

std::string ms_to_time(unsigned long jiffies) {
  int seconds = (jiffies / sysconf(_SC_CLK_TCK));
  int minutes = ((int)seconds / 60) % 60;
  int hours = ((int)seconds / 60 / 24) % 24;
  char temp[20];
  sprintf(temp, "%02d:%02d:%02d", hours, minutes, seconds % 60);
  return temp;
}

/*
 * Converts epoch seconds to time_stamp
 */

std::string get_stamp(unsigned long long epoch) {
  std::ifstream infile;
  infile.open("/proc/stat");
  unsigned long long btime;
  if (infile.is_open()) {
    std::string line;
    while (std::getline(infile, line)) {
      std::istringstream iss(line);
      std::string key;
      std::string value;
      iss >> key;
      iss >> value;
      if (!key.compare("btime")) {
        btime = std::stoull(value);
        break;
      }
    }
    std::time_t total = btime + epoch;
    infile.close();
    std::stringstream ss;
    ss << std::put_time(std::localtime(&total), "%c %Z");
    return ss.str();
  } else {
    return "boot time not found";
  }
  return "get_stamp error";
}

/*
 * Returns proc_prop of given pid
 */

proc_prop details(int pid) {
  proc_prop ret;
  std::ifstream infile;
  infile.open("/proc/" + std::to_string(pid) + "/status");
  if (infile.is_open()) {
    std::string line;
    int got = 0;
    unsigned long rss;
    unsigned long swap;
    unsigned long rss_file;
    unsigned long rssshmem;
    unsigned long long start_time;
    while ((std::getline(infile, line)) || (got < 8)) {
      std::istringstream iss(line);
      std::string key;
      std::string value;
      std::string extra;
      iss >> key;
      iss >> value;
      iss >> extra;
      key.pop_back();
      if (!key.compare("Name")) {
        ret.name = value;
        got++;
      } else if (!key.compare("Uid")) {
        int userid = stoi(value);
        std::string temp = string_uid(userid) + "(" +
                            std::to_string(userid) + ")";
        ret.user = temp;
        got++;
      } else if (!key.compare("State")) {
        std::string temp = extra;
        ret.status = temp.substr(1, temp.length()-2);
        got++;
      } else if (!key.compare("VmRSS")) {
        rss = std::stoul(value, 0);
        got++;
      } else if (!key.compare("VmSwap")) {
        swap = std::stoul(value, 0);
        got++;
      } else if (!key.compare("VmSize")) {
        ret.v_mem = std::stoul(value, 0);
        got++;
      } else if (!key.compare("RssFile")) {
        rss_file = std::stoul(value, 0);
        got++;
      } else if (!key.compare("RssShmem")) {
        rssshmem = std::stoul(value, 0);
        got++;
      }
    }
    infile.close();
    FILE *f = fopen(("/proc/" + std::to_string(pid) + "/stat").c_str(), "r");
    if (f != NULL) {
      int items = fscanf(f,"%*d %*[^)] %*c %*c %*d %*d %*d %*d %*d %*d %*d "
            "%*d %*d %*d %*d %*d %*d %*d %*d %d %*d %*d %llu",
            &(ret.nice), &start_time);
      if (items != 2) {
        std::cout << "details: error reading file\n";
        ret.cpu = 0;
        ret.uptime = "0";
        ret.id = 0;
        ret.memory = 0;
        ret.sh_mem = 0;
        ret.started = 0;
        return ret;
      }
      unsigned long procstart = pid_time(std::to_string(pid));
      unsigned long cpu_start = cpu_time();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      unsigned long cpu_end = cpu_time();
      unsigned long procend = pid_time(std::to_string(pid));
      int cpus = get_nprocs_conf();
      ret.cpu = ((float)100 * (float)(procend - procstart) /
                  (float)(cpu_end - cpu_start)) * cpus;
      ret.uptime = ms_to_time(procend);
      ret.id = pid;
      ret.memory = rss + swap;
      ret.sh_mem = rss_file + rssshmem;
      ret.started = get_stamp(start_time / sysconf(_SC_CLK_TCK));
      fclose(f);
    } else {
      std::cout << "cpu open fail\n";
      return ret;
    }
  } else {
    std::cout << "details: error opening file\n";
    return ret;
  }
  return ret;
}

/*
 * Prints proc details for debugging
 */

void printdetails(proc_prop details) {
  std::cout << details.name << " " << details.id << " " << details.user << " "
            << details.status << " " << details.memory << " " << details.sh_mem
            << " " << details.cpu << " " << details.uptime << " "
            << details.started << " " << details.nice << std::endl;
}

/*
 * Sample main for debugging
 */

// int main(int argc, const char** argv) {
//   proc_prop aaaahhhhhh = details(3766);
//   printdetails(aaaahhhhhh);
//   // std::list<mem_read> wish90 = mem_map(3254);
//   // print_list(wish90);
//   //free_list(wish90);
// }