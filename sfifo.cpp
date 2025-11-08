#include "sfifo.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <fstream>
#include <iostream>

int sfifo_mkfifo(std::string filename) {
    std::filesystem::path path = PATH_ROOT / std::filesystem::path(filename);
    std::error_code err;
    int ret;

    if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path << " exists and is NOT a FIFO pipe" << std::endl;
            exit(1);
        } else {
            if (!std::filesystem::remove(path, err)) {
                std::cout << err << std::endl << path \
                    << " exists, failure when attempting to remove and recreate" << std::endl;
                exit(1);
            }
        }
    }
    if ((ret = mkfifo(path.c_str(), 0660)))
        PERROR_EXIT_RET("mkfifo", ret);
    return 0;
}

int sfifo_open(std::string filename, int mode)
{
    std::filesystem::path path = PATH_ROOT / std::filesystem::path(filename);
    return open(path.c_str(), O_TRUNC | mode | O_NONBLOCK);
}

std::fstream sfifo_fstream(std::string filename) {
    std::filesystem::path path = PATH_ROOT / std::filesystem::path(filename);

    std::fstream fifo(path.c_str());
    if (!fifo.is_open()) {
        std::cout << "Failed to open fifo fstream at " << path <<std::endl;
        exit(1);
    }
    return fifo;
}

int get_pid_max()
{
    std::ifstream pid_max_fstream("/proc/sys/kernel/pid_max");
    int v;
    pid_max_fstream >> v;
    return v;
}

std::string get_cli_filename(int pid)
{
    return CLIENT_PREFIX + std::to_string(pid);
}

std::string get_cli_filename(std::string pid)
{
    return CLIENT_PREFIX + pid;
}
