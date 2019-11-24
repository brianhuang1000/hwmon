#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>

// format strings for cpu /proc/stat cpu usage lines

#define CPU_LINE_FMT "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld"
#define CORE_LINE_FMT "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld"

// format string for /proc/net/dev lines
#define NET_LINE_FMT "%*s %lu %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d"


#define PROC_LINE_LEN 256

std::vector<std::vector<long>> prev_cpu_usage;
std::vector<unsigned long> prev_net_usage(2);
auto prev_network_time = std::chrono::system_clock::now();

// CPU USAGE

enum Mode {
    usr_mode,
    nice_mode,
    kernel_mode,
    idle_mode,
    iowait_mode,
    irq_mode,
    softirq_mode,
    steal_mode,
    guest_mode,
    guest_nice_mode
};

bool init_cpu_monitor() {
    std::ifstream procstat;
    procstat.open("/proc/stat");
    if (!procstat) {
        return false;
    }

    char line[PROC_LINE_LEN];
    std::vector<long> stats(10, 0L);

    // get total cpu usage

    procstat.getline(line, PROC_LINE_LEN);
    sscanf(line, CPU_LINE_FMT,
        &stats.at(usr_mode),
        &stats.at(nice_mode),
        &stats.at(kernel_mode),
        &stats.at(idle_mode),
        &stats.at(iowait_mode),
        &stats.at(irq_mode),
        &stats.at(softirq_mode),
        &stats.at(steal_mode),
        &stats.at(guest_mode),
        &stats.at(guest_nice_mode)
    );
    prev_cpu_usage.push_back(stats);

    procstat.getline(line, PROC_LINE_LEN);

    while (!procstat.fail()) {
        int core_num = 0;
        sscanf(line, CORE_LINE_FMT,
            &core_num,
            &stats.at(usr_mode),
            &stats.at(nice_mode),
            &stats.at(kernel_mode),
            &stats.at(idle_mode),
            &stats.at(iowait_mode),
            &stats.at(irq_mode),
            &stats.at(softirq_mode),
            &stats.at(steal_mode),
            &stats.at(guest_mode),
            &stats.at(guest_nice_mode)
        );
        prev_cpu_usage.push_back(stats);
        procstat.getline(line, PROC_LINE_LEN);
    }

    procstat.close();
    return true;
}

std::vector<float> get_cpu_usage() {
    std::ifstream procstat;
    procstat.open("/proc/stat");
    if (!procstat) {
        return std::vector<float>(0);
    }

    char line[PROC_LINE_LEN];
    std::vector<long> stats(10, 0L);
    std::vector<std::vector<long>> cur_cpu_usage;

    // get total cpu usage

    procstat.getline(line, PROC_LINE_LEN);
    sscanf(line, CPU_LINE_FMT,
        &stats.at(usr_mode),
        &stats.at(nice_mode),
        &stats.at(kernel_mode),
        &stats.at(idle_mode),
        &stats.at(iowait_mode),
        &stats.at(irq_mode),
        &stats.at(softirq_mode),
        &stats.at(steal_mode),
        &stats.at(guest_mode),
        &stats.at(guest_nice_mode)
    );
    cur_cpu_usage.push_back(stats);

    procstat.getline(line, PROC_LINE_LEN);

    while (!procstat.fail()) {
        int core_num = 0;
        sscanf(line, CORE_LINE_FMT,
            &core_num,
            &stats.at(usr_mode),
            &stats.at(nice_mode),
            &stats.at(kernel_mode),
            &stats.at(idle_mode),
            &stats.at(iowait_mode),
            &stats.at(irq_mode),
            &stats.at(softirq_mode),
            &stats.at(steal_mode),
            &stats.at(guest_mode),
            &stats.at(guest_nice_mode)
        );
        cur_cpu_usage.push_back(stats);
        procstat.getline(line, PROC_LINE_LEN);
    }

    procstat.close();

    //std::vector<std::vector<long>> delta;
    std::vector<float> out;
    for(unsigned long i = 0; i < cur_cpu_usage.size(); i++) {
        //delta.push_back(std::vector<long>(10));
        long total_time = 0;
        long idle_time = 0;
        for (unsigned long j = 0; j < cur_cpu_usage.at(i).size(); j++) {
            /*delta.at(i).at(j) = cur_cpu_usage.at(i).at(j) -
                                prev_cpu_usage.at(i).at(j);*/
            total_time += cur_cpu_usage.at(i).at(j) - prev_cpu_usage.at(i).at(j);
            if(j == idle_mode || j == iowait_mode) {
                idle_time += cur_cpu_usage.at(i).at(j) - prev_cpu_usage.at(i).at(j);
            }
        }
        out.push_back(1 - ((float)idle_time / total_time));
    }

    prev_cpu_usage = cur_cpu_usage;
    return out;
}

void print_cpu_stats(std::vector<std::vector<long>> stats) {
    std::cout << "========== CPU USAGE STATS ==========" << '\n';
    for(unsigned long i = 0; i < stats.size(); i++) {
        std::cout << (i == 0 ? "Total " : "Core ");
        if(i != 0) {
            std::cout << i << ' ';
        }
        for(unsigned long j = 0; j < stats.at(i).size(); j++) {
            std::cout << "| " << stats.at(i).at(j);
        }
        std::cout << '\n';
    }
    std::cout << "=====================================" << '\n';
}

void print_prev_cpu_stats() {
    print_cpu_stats(prev_cpu_usage);
}

void print_cpu_usage(std::vector<float> usage) {
    std::cout << "========== CPU USAGE  ==========" << '\n';
    for(unsigned long i = 0; i < usage.size(); i++) {
        std::cout << (i == 0 ? "Total " : "Core ");
        if (i != 0) {
            std::cout << i;
        }
        std::cout << ": " << usage.at(i) * 100 << "%\n";
    }
    std::cout << "================================" << '\n';
}

// MEMORY & SWAP USAGE

std::vector<unsigned long> get_mem_usage() {
    std::ifstream meminfo;
    std::vector<unsigned long> out(4);
    meminfo.open("/proc/meminfo");
    if (!meminfo) {
        return std::vector<unsigned long>(0);
    }

    char line[PROC_LINE_LEN];
    do {
        meminfo.getline(line, PROC_LINE_LEN);
        if (meminfo.fail()) {
            std::cout << "fail" << '\n';
            break;
        }
        if (strncmp(line, "MemTotal", 8) == 0) {
            sscanf(line, "MemTotal: %lu", &out.at(1));
        }
        else if (strncmp(line, "MemFree", 7) == 0) {
            sscanf(line, "MemFree: %lu", &out.at(0));
        }
        else if (strncmp(line, "SwapTotal", 9) == 0) {
            sscanf(line, "SwapTotal: %lu", &out.at(3));
        }
        else if (strncmp(line, "SwapFree", 8) == 0) {
            sscanf(line, "SwapFree: %lu", &out.at(2));
        }
    } while (strncmp(line, "SwapFree", 8) != 0);
    return out;
}

void print_mem_usage(std::vector<unsigned long> usage) {
    if(usage.size() != 4) {
        std::cout << "Invalid memory stats size: " << usage.size() << '\n';
        return;
    }

    std::cout << "========== MEMORY USAGE ==========" << '\n';
    std::cout << "Memory used: " << usage.at(0) << '/' << usage.at(1) << " kB (" << (double)usage.at(0)/usage.at(1)*100 << "%)" << '\n';
    std::cout << "Swap used: " << usage.at(2) << '/' << usage.at(3) << " kB (" << (double)usage.at(2)/usage.at(3)*100 << "%)" << '\n';
    std::cout << "==================================" << '\n';
}

// NETWORK USAGE

bool init_network_monitor() {
    auto network_time = std::chrono::system_clock::now();
    prev_network_time = network_time;
    std::vector<unsigned long> current_net_usage;
    std::ifstream netdev;

    netdev.open("/proc/net/dev");
    if(!netdev) {
        return false;
    }

    char line[PROC_LINE_LEN];
    short i = 0;
    //double bytes_sent = 0;
    //double bytes_received = 0;
    unsigned long sent_total = 0, recv_total = 0;

    while(!netdev.fail()) {
        netdev.getline(line, PROC_LINE_LEN);
        if(i < 2) {
            ++i;
            continue;   //First two lines are column headers
        }
        //unsigned long sent, recv;
        unsigned long sent, recv;
        sscanf(line, NET_LINE_FMT, &recv, &sent);
        recv_total += recv;
        sent_total += sent;
        //std::cout << "Line: " << line << '\n';
        //std::cout << "Sent: " << sent << ", Received: " << recv << '\n';
    }
    current_net_usage.push_back(sent_total);
    current_net_usage.push_back(recv_total);

    prev_net_usage = current_net_usage;
    return true;
}

std::vector<double> get_network_usage() {
    std::vector<double> out;
    std::vector<unsigned long> current_net_usage;
    std::ifstream netdev;

    netdev.open("/proc/net/dev");
    if(!netdev) {
        return std::vector<double>(0);
    }
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> seconds = now - prev_network_time;
    prev_network_time = now;

    char line[PROC_LINE_LEN];
    short i = 0;
    
    unsigned long sent_total = 0, recv_total = 0;

    //std::cout << sent_total << ' ' << recv_total << '\n';

    while(!netdev.fail()) {
        netdev.getline(line, PROC_LINE_LEN);
        if(i < 2) {
            ++i;
            continue;   //First two lines are column headers
        }
        //unsigned long sent, recv;
        unsigned long sent, recv;
        sscanf(line, NET_LINE_FMT, &recv, &sent);
        recv_total += recv;
        sent_total += sent;
        //std::cout << "Line: " << line << '\n';
        //std::cout << "Sent: " << sent << ", Received: " << recv << '\n';
    }
    current_net_usage.push_back(sent_total);
    current_net_usage.push_back(recv_total);

    out.push_back(current_net_usage.at(0) - prev_net_usage.at(0));
    out.push_back(current_net_usage.at(1) - prev_net_usage.at(1));

    prev_net_usage = current_net_usage;

    out.push_back(seconds.count());
    //std::cout << seconds.count() << " seconds";

    netdev.close();
    return out;
}

void print_net_usage(std::vector<double> usage) {
    std::cout << "========== NETWORK USAGE ==========" << '\n';
    std::cout << "Sent " << usage.at(0) << " bytes\nReceived " << usage.at(1) << " bytes\nIn the last " << usage.at(2) << " seconds" << '\n';
    std::cout << "===================================" << '\n';
}

// INIT

bool init_resource_monitor() {
    bool cpu = init_cpu_monitor();

    // TODO memory
    bool mem = true;

    // TODO network
    bool net = init_network_monitor();

    return cpu && mem && net;
}