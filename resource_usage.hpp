#include <vector>

// Initialize starting values in all resource monitors
// Returns false on error, true otherwise
bool init_resource_monitor();

// Get the average cpu usage since the last time get_cpu_usage was called
// or since init_resource_monitor was called, if this is the first time calling
// get_cpu_usage
// Returns a std::vector where the 0th element is the total cpu usage
// and all subsequent elements are the usage of the nth core.
std::vector<float> get_cpu_usage();

// Print the latest cpu stats taken from /proc/stat to stdout
void print_prev_cpu_stats();