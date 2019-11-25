#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP


#include <vector>
#include <string>

class FileSystem
{
    public:
        FileSystem(std::string name, std::string dir, std::string type);
        std::string name;
        std::string dir;
        std::string type;
        unsigned int block_size;
        unsigned long total_blocks;
        unsigned long available_blocks;
        unsigned long used_blocks;
        unsigned long free_blocks;
        double get_usage_percent();
        void print_info();
};

// Returns a list of FileSystem objects corresponding to the
// user's mounted file systems.
// Does not filter out file systems with 0 total space (proc, cgroup etc.)
// Filter its output before displaying in UI
// Otherwise there will be a bunch of file systems that are NaN% full
std::vector<FileSystem> get_fs_list();


#endif // FILESYSTEM_HPP
