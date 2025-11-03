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

int main()
{
    std::string pid;
    std::filesystem::path path, root, tmp_filename;
    std::string line, msg;
    char c;
    int char_count;
    std::fstream cli;
    int srv_fd;

    srv_fd = sfifo_open("srv");
    if (srv_fd == -1) {
        perror("open");
        exit(1);
    }
    FILE* fp = fdopen(srv_fd, "r");

    while (true){
        while ((c = getc(fp)) == -1) {
                usleep(1000);
        }
        do {
            pid.append(1, c);
        } while ((c = getc(fp)) != '\0');
        cli = sfifo_fstream(pid);

        char_count = 0;
        while (getc(fp) != '\0')
                 char_count++;
        std::cout << char_count << std::endl;
        cli << char_count << std::endl;
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

