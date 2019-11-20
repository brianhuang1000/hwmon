#include "resource_usage.hpp"
#include <vector>
#include <iostream>
#include <unistd.h>

#define SLEEP_TIME 1 //seconds

int main() {
    init_resource_monitor();
    //print_prev_cpu_stats();
    while(true) {
        sleep(SLEEP_TIME);
        std::vector<float> usage = get_cpu_usage();
        //std::cout << usage.size();
        for(unsigned long i = 0; i < usage.size(); i++) {
            std::cout << usage.at(i) << ' ';
        }
        std::cout << '\n';
    }
}