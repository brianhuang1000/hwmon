#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string.h>

/* notes

Use smaps for  memory maps
use fd for file descriptors < readlink in unistd.h to find location of file

Things that need to refresh
  :bool remove, cpu percentage, status, memory

*/
float get_cpu(std::string pid);


int main(int argc, const char** argv) {
  unsigned long utime;
  unsigned long stime;
  unsigned long cutime;
  unsigned long cstime;
  FILE *f = fopen(("/proc/1139/stat"), "r");
  char junk[255] = " ";
  while (strstr(junk,")") == NULL) {
    if(fscanf(f,"%[^ ]%*c",junk) ==  EOF){
      break;
    };
    std::cout << junk <<std::endl;
  }
  int number = fscanf(f,"%*c %*d %*d %*d %*d %*d %*d %*d "
            "%*d %*d %*d %lu %lu %lu %lu", &utime, &stime, &cutime, &cstime);
  std::cout<< utime <<" "<<stime<<" "<<cutime<<" "<<cstime << " " << number<<std::endl;
}

void horahora() {
  auto start = std::chrono::high_resolution_clock::now();
  FILE *f = fopen("/proc/2229/stat", "r");
  int pid;
  char *filename = (char *)malloc(255);
  char state;
  int ppid;
  unsigned long utime;
  unsigned long stime;
  unsigned long cutime;
  unsigned long cstime;

  fscanf(f,"%d %[^)] %*c %c %d %*d %*d %*d %*d %*d %*d "
          "%*d %*d %*d %lu %lu %lu %lu",
    &pid, filename, &state, &ppid, &utime, &stime, &cutime, &cstime);
  std::cout << pid << " " << filename + 1 << " " << state << " " << ppid << " "
            << stime << " " << utime << " " << cutime << " " << cstime << "\n";
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
  std::cout << duration.count() << std::endl;
}

unsigned long cpu_time(){
  FILE *f = fopen("/proc/stat", "r");
  unsigned long cpus[10];
  fscanf(f,"%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
          &cpus[0], &cpus[1], &cpus[2], &cpus[3], &cpus[4], &cpus[5], &cpus[6],
          &cpus[7], &cpus[8], &cpus[9]);
  unsigned long total = 0;
  for (int i = 0; i < 10; i++) {
    //std::cout << cpus[i] << " ";
    total += cpus[i];
  }
  //std::cout << std::endl;
  //std::cout << "global cpu: " << total << std::endl;
  fclose(f);
  return total;
}

float get_cpu(std::string pid) {
  FILE *f = fopen(("/proc/" + pid + "/stat").c_str(), "r");
  unsigned long utime;
  unsigned long stime;
  unsigned long cutime;
  unsigned long cstime;
  fscanf(f,"%*d %*[^)] %*c %*c %*d %*d %*d %*d %*d %*d %*d "
          "%*d %*d %*d %lu %lu %lu %lu", &utime, &stime, &cutime, &cstime);
  unsigned long process_total = utime + stime + cutime + cstime;
  unsigned long start_cpu = cpu_time();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  rewind(f);
  fscanf(f,"%*d %*[^)] %*c %*c %*d %*d %*d %*d %*d %*d %*d "
          "%*d %*d %*d %lu %lu %lu %lu", &utime, &stime, &cutime, &cstime);
  unsigned long process_after = utime + stime + cutime + cstime;
  unsigned long end_cpu = cpu_time();
  //std::cout << process_after << " " << process_total << std::endl;
  fclose(f);
  return ((float)100 * 8 * (process_after-process_total)/(end_cpu-start_cpu));
}