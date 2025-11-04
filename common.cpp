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

int sfifo_mkfifo(std::filesystem::path path) {
        if (mkfifo(path.c_str(), 0660)) {
        std::perror("mkfifo");
       exit(1);
    }
    return 0;
}

int sfifo_open(std::string filename)
{
    std::filesystem::path fname = filename;
    std::filesystem::path path = PATH_ROOT / fname;
    std::error_code err;
    int fd;

    if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path << " exists and is NOT a FIFO pipe" << std::endl;
            exit(1);
        } else {
            if (!std::filesystem::remove(path, err)) {
                std::cerr << err << std::endl << path \
                    << " exists, failure when attempting to remove and recreate" << std::endl;
                exit(1);
            }
        }
    }
    if (mkfifo(path.c_str(), 0660)) {
        std::perror("mkfifo");
       exit(1);
    }
    return open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK);
}

std::fstream sfifo_fstream(std::filesystem::path filename) {

    std::filesystem::path path = PATH_ROOT / filename;
    if (std::filesystem::exists(path) && !std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path \
            << " exists and is NOT a FIFO pipe" << std::endl;
            exit(1);
    } else if (!std::filesystem::exists(path)) {
        if (mkfifo(path.c_str(), 0660)) {
        std::perror("mkfifo");
       exit(1);
        }
    }
    std::fstream fifo(path.c_str());
    if (!fifo.is_open()) {
        std::cout << "Failed to open Fcreate_and_read_IFO at " << path <<std::endl;
        exit(1);
    }
    return fifo;
}

