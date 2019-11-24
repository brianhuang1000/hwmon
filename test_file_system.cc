#include "FileSystem.hpp"

int main() {
    std::vector<FileSystem> test = get_fs_list();
    for(unsigned long i = 0; i < test.size(); ++i) {
        test.at(i).print_info();
    }
}