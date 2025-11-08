#include "sfifo.h"

#include <unistd.h>

#include <fstream>
#include <iostream>

int main(void)
{
    std::string line;
    std::cout << ">> Enter your message in one line, press ENTER when done." << std::endl;
    while (1) {
        getline(std::cin, line);
        if (line.length() <= ARG_MAX)
            break;
        std::cout << "Message exceeding " << ARG_MAX << " chars, try again." << std::endl;
    }
    std::filesystem::path pid = std::to_string(getpid());
    std::string payload = pid.string() + '\0' + line + '\0';

    int srv_fd = sfifo_open(PATH_ROOT, "srv");
    if (srv_fd == -1)
        PERROR_EXIT("open");
    sfifo_mkfifo(PATH_ROOT, pid);
    int sz;
    if ((sz = write(srv_fd, payload.data(), payload.size()) < -1))
        PERROR_EXIT("write");

    std::fstream cli = sfifo_fstream(PATH_ROOT, pid);
    std::string char_count;
    cli >> char_count;
    std::cout << "Char count: " << char_count << std::endl;
    cli.close();
    close(srv_fd);
    return 0;
}
