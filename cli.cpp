#include "common.h"

#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/un.h>


int main(void)
{
    std::filesystem::path pid;
    std::string line, msg, payload;
    std::fstream srv, cli;
    int srv_fd;
    std::string n_of_chars;

    pid = std::to_string(getpid());

    std::cout << ">> Enter your message, one or more lines separated by ENTER. " \
        << "Press CTRL+D when done" << std::endl;
    while (getline(std::cin, line))
        msg.append(line).append("\n");
    payload = pid.string() + '\0' + msg + '\0';
    srv = sfifo_fstream(PATH_ROOT, "srv");
    srv << payload;
    std::cout << ">> Sent!" << std::endl;

    // TODO: replace with poll
    while (!std::filesystem::exists(PATH_ROOT / pid))
        usleep(1000);
    // TODO: close?
    cli = sfifo_fstream(PATH_ROOT, pid);
    std::cout << "Number of chars: " << cli.rdbuf();
    return 0;
}
