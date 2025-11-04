#include "common.h"

#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

int sfifo_mkfifo(std::string path_root, std::string filename) {
    std::filesystem::path path = path_root / std::filesystem::path(filename);
    std::error_code err;
    int ret;

    if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path << " exists and is NOT a FIFO pipe" << std::endl;
            exit(1);
        } else {
            // TODO: check with sudo
            if (!std::filesystem::remove(path, err)) {
                std::cout << err << std::endl << path \
                    << " exists, failure when attempting to remove and recreate" << std::endl;
                exit(1);
            }
        }
    }
    if ((ret = mkfifo(path.c_str(), 0660))) {
        std::perror("mkfifo");
        exit(ret);
    }
    return 0;
}

int sfifo_open(std::string path_root, std::string filename)
{
    std::filesystem::path path = path_root / std::filesystem::path(filename);
    int fd;

    // TODO: err handling where used
    return open(path.c_str(), O_RDWR | O_TRUNC | O_NONBLOCK);
}

std::fstream sfifo_fstream(std::string path_root, std::string filename) {
    std::filesystem::path path = path_root / std::filesystem::path(filename);

    std::fstream fifo(path.c_str());
    if (!fifo.is_open()) {
        std::cout << "Failed to open fifo fstream at " << path <<std::endl;
        exit(1);
    }
    return fifo;
}

