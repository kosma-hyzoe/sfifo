#include "common.h"

#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
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
    std::fstream srv, cli;

    root = PATH_ROOT;
    srv = get_fifo(root, "srv", true);

    while (1){
        srv.get(); // first char is supposed to be null, so skippable
        std::cout << "dupa";

        // pid = "";
        // while ((c = srv.get()) != '\0')
        //     pid.append(1, c);
        // while ((c = srv.get()) != '\0')
        //     char_count++;
        // cli = get_fifo(root, pid, false);
        // cli << char_count;
    }

    while (std::getline(srv, line)) {
        size_t pos = line.find('\0');
        if (pos == std::string::npos) {
                usleep(1000);
                continue;
        }


        char_count = 0;
        while (srv.get() != '\0')
            char_count++;
        cli = get_fifo(root, pid, false);
        cli << char_count;
    }

    return 0;
}

