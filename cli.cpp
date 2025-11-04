#include "common.h"

#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/un.h>

void dbg()
{
    std::cout << "foobar" << std::endl;
}

int main(void)
{
    std::filesystem::path pid;
    std::string line, msg, payload;
    std::fstream srv, cli;
    int sz;
    int srv_fd;
    std::string n_of_chars;

    pid = std::to_string(getpid());

    std::cout << ">> Enter your message, one or more lines separated by ENTER. " \
        << "Press CTRL+D when done" << std::endl;
    while (getline(std::cin, line))
        msg.append(line).append("\n");
    payload = pid.string() + '\0' + msg + '\0';

    srv_fd = sfifo_open(PATH_ROOT, "srv");
    if (srv_fd == -1) {
        perror("open");
        exit(1);
    }
    if ((sz = write(srv_fd, payload.data(), payload.size()) < -1)) {
        perror("write");
        exit(1);
    }
    std::cout << ">> Sent!" << std::endl;

    // TODO: replace with poll
    while (!std::filesystem::exists(PATH_ROOT / pid))
        usleep(1000);
    // TODO: close?
    cli = sfifo_fstream(PATH_ROOT, pid);
    std::cout << "Number of chars: " << cli.rdbuf();
    return 0;
}
