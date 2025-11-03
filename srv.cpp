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

int get_fifo(std::string filename, int mode)
{
    std::filesystem::path fname = filename;
    std::filesystem::path path = PATH_ROOT / fname;
    int fd;

    if (std::filesystem::exists(path) &&!std::filesystem::is_fifo(path)) {
        std::cout << "ERROR: File " << path << " exists and is NOT a FIFO pipe" << std::endl;
        exit(1);
    }
    if (mode == CREATE) {
        if (mkfifo(path.c_str(), 0660)) {
            std::perror("mkfifo");
            exit(1);
        }
        return open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    } else {
        return open(path.c_str(), O_RDONLY);
    }
}

int main()
{
    std::string pid;
    std::filesystem::path path, root, tmp_filename;
    std::string line, msg;
    char c;
    int char_count;
    std::fstream srv, cli;

    root = PATH_ROOT;
    srv = get_fifo("srv");
    int srv_fd = open(PATH_ROOT "/srv", O_RDONLY | O_NONBLOCK);
    FILE* fp = fdopen(srv_fd, "r");

    while (true){
        while ((c = getc(fp)) == -1) {
                usleep(1000);
        }
        do {
            pid.append(1, c);
        } while ((c = getc(fp)) != '\0');

        char_count = 0;
        while (getc(fp) != '\0')
                 char_count++;
        std::cout << pid << "\t" << char_count << std::endl;
        }
}
        // while ((c = srv.get()) != '\0')
        // while ((c = srv.get()) != '\0')
        //     char_count++;
        // cli = get_fifo(root, pid, false);
        // cli << char_count;
    // }

    // while (std::getline(srv, line)) {
    //     size_t pos = line.find('\0');
    //     if (pos == std::string::npos) {
    //             usleep(1000);
    //             continue;
    //     }
    //
    //
    //     char_count = 0;
    //     while (srv.get() != '\0')
    //         char_count++;
    //     cli = get_fifo(root, pid, false);
    //     cli << char_count;
    // }

//     return 0;
// }

