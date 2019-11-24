#include "resource_usage.hpp"
#include <vector>
#include <iostream>
#include <unistd.h>

#define SLEEP_TIME 1 //seconds

int main() {
    init_resource_monitor();
    while(true) {
        sleep(SLEEP_TIME);
        // std::vector<float> cpu_usage = get_cpu_usage();
        // std::vector<unsigned long> mem_usage = get_mem_usage();
        std::vector<double> net_usage = get_network_usage();
        
        // print_cpu_usage(cpu_usage);
        // print_mem_usage(mem_usage);
        print_net_usage(net_usage);
        std::cout << '\n';
    }
}