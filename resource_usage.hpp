#include <vector>

// Initialize starting values in all resource monitors
// Returns false on error, true otherwise
bool init_resource_monitor();
bool init_network_monitor();
bool init_cpu_monitor();

// Get the average cpu usage since the last time get_cpu_usage was called
// or since init_resource_monitor was called, if this is the first time calling
// get_cpu_usage
// Returns a std::vector<float> where the 0th element is the total cpu usage
// and all subsequent elements are the usage of the nth core.
// All elements will be between 0 and 1, where 0 is 0% usage and 1 is 100%
std::vector<float> get_cpu_usage();

// Print the latest cpu stats taken from /proc/stat to stdout
void print_prev_cpu_stats();

// Print the result of get_cpu_usage() to stdout
void print_cpu_usage(std::vector<float>);

// Get the current memory & swap usage of the system
// Returns a std::vector<unsigned long> with the following elements:
// 0: Memory used (kB)
// 1: Memory total (kB)
// 2: Swap space used (kB)
// 3: Swap space total (kB)
std::vector<unsigned long> get_mem_usage();

// Print the result of get_mem_usage() to stdout
void print_mem_usage(std::vector<unsigned long>);

// Get the current network usage of the system
// Returns a std::vector<double> with the following elements:
// 0: Bytes sent since last call
// 1: Bytes received since last call
// 2: Seconds elapsed since last call
std::vector<double> get_network_usage();

// Print the result of get_net_usage() to stdout
void print_net_usage(std::vector<double>);
