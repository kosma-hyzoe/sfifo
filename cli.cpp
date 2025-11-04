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


int main(void)
{
    std::filesystem::path pid;
    std::filesystem::path path, root, fullpath;
    std::string tmp;
    std::string line, msg;
    int cli_fd, srv_fd;
    char c;
    std::string n_of_chars;


    pid = std::to_string(getpid());
    std::fstream srv = sfifo_fstream("srv");
    cli_fd = sfifo_open(pid);
    if (cli_fd == -1) {
        perror("open");
        exit(1);
    }
    FILE* fp = fdopen(srv_fd, "r");
    std::cout << ">> Enter your message, one or more lines separated by ENTER. " \
        << "Press CTRL+D when done" << std::endl;
    while (getline(std::cin, line))
        msg.append(line).append("\n");
    srv <<  pid.string() << '\0' << std::endl << msg << '\0';
    std::cout << ">> Sent!" << std::endl;

    const int bufferSize = 256;
    char buffer[bufferSize];

    while ((c = getc(fp)) == -1)
            usleep(1000);
    n_of_chars = "";
    while ((c = getc(fp)) != EOF)
        n_of_chars.append(1, c);
    std::cout << n_of_chars << std::endl;

    // };
    // std::getline(cli, line);
    // std::cout << line << std::endl;
    return 0;
}
