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
    std::string n_of_chars;


    pid = std::to_string(getpid());
    std::fstream srv = sfifo_fstream("srv");
    std::fstream cli = sfifo_fstream(pid);

    std::cout << ">> Enter your message, one or more lines separated by ENTER. " \
        << "Press CTRL+D when done" << std::endl;
    while (getline(std::cin, line))
        msg.append(line).append("\n");
    srv <<  pid.string() << '\0' << std::endl << msg << '\0';
    std::cout << ">> Sent!" << std::endl;

    const int bufferSize = 256;
    char buffer[bufferSize];

    while (cli.peek() == EOF) {
        usleep(1000);
    };
    std::getline(cli, line);
    std::cout << line << std::endl;
    return 0;
}
