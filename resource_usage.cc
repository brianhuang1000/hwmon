#include <vector>
#include <fstream>
#include <iostream>

// format strings for cpu /proc/stat cpu usage lines

#define CPU_LINE_FMT "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld"
#define CORE_LINE_FMT "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld"
#define PROCSTAT_LINE_LEN 256

std::vector<std::vector<long>> prev_cpu_usage;

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

    char line[PROCSTAT_LINE_LEN];
    std::vector<long> stats(10, 0L);

    // get total cpu usage

    procstat.getline(line, PROCSTAT_LINE_LEN);
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

    procstat.getline(line, PROCSTAT_LINE_LEN);

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
        procstat.getline(line, PROCSTAT_LINE_LEN);
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

    char line[PROCSTAT_LINE_LEN];
    std::vector<long> stats(10, 0L);
    std::vector<std::vector<long>> cur_cpu_usage;

    // get total cpu usage

    procstat.getline(line, PROCSTAT_LINE_LEN);
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

    procstat.getline(line, PROCSTAT_LINE_LEN);

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
        procstat.getline(line, PROCSTAT_LINE_LEN);
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

/*std::vector<float> get_cpu_usage() {
    return std::vector<float>(1,1);
}*/

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

bool init_resource_monitor() {
    bool cpu = init_cpu_monitor();

    // TODO memory
    bool mem = true;

    // TODO network
    bool net = true;

    return cpu && mem && net;
}