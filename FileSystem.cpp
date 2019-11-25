#include "FileSystem.hpp"
#include <sys/statvfs.h>
#include <mntent.h>
#include <iostream>
#include <cstring>

FileSystem::FileSystem(std::string name, std::string dir, std::string type) {
    this->dir = dir;
    this->name = name;
    this->type = type;
    struct statvfs buf;
    if (statvfs(dir.c_str(), &buf) != 0) {
        std::cout << "statvfs() error\n";
        return;
    }
    this->block_size = buf.f_bsize;
    this->total_blocks = buf.f_blocks;
    this->available_blocks = buf.f_bavail;
    this->used_blocks = buf.f_blocks - buf.f_bfree;
    this->free_blocks = buf.f_bfree;
}

double FileSystem::get_usage_percent() {
    return (1 - (double)this->available_blocks / this->total_blocks) * 100;
}

void FileSystem::print_info() {
    std::cout << this->name << " (" << this->type << ", mounted at " << this->dir << ")\n";
    std::cout << '\t' << this->block_size * this->total_blocks << " bytes total\n"
              << '\t' << this->block_size * this->used_blocks << " bytes used\n"
              << '\t' << this->block_size * this->free_blocks << " bytes free\n"
              << '\t' << this->block_size * this->available_blocks << " bytes available\n"
              << '\t' << this->get_usage_percent() << "% filled\n";
}

std::vector<FileSystem> get_fs_list() {
    std::vector<FileSystem> out;
    FILE *fp = setmntent("/proc/mounts", "r");
    struct mntent *ENT;
    do {
        ENT = getmntent(fp);
        if(ENT == NULL) {
            break;
        }
        out.push_back(FileSystem(ENT->mnt_fsname, ENT->mnt_dir, ENT->mnt_type));
    } while (1);
    endmntent(fp);
    return out;
}
