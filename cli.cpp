#include "sfifo.h"

#include <unistd.h>
#include <fcntl.h>

#include <fstream>
#include <iostream>

int main(void)
{
    if (!std::filesystem::exists(SRV_PATH)) {
        std::cout << SRV_PATH << " does not exist. Is the server running?\n";
        exit(1);
    }
    std::string line;
    std::cout << ">> Enter your message in one line, press ENTER when done." << std::endl;
    while (1) {
        getline(std::cin, line);
        if (line.length() <= static_cast<std::string::size_type>(ARG_MAX))
            break;
        std::cout << "Message exceeding " << ARG_MAX << " chars, try again." << std::endl;
    }
    std::filesystem::path pid = std::to_string(getpid());
    std::string payload = pid.string() + '\0' + line + '\0';

    int srv_fd = sfifo_open(SRV_PATH, O_WRONLY);
    if (srv_fd == -1)
        PERROR_EXIT("open");
    sfifo_mkfifo(pid);
    int sz;
    if ((sz = write(srv_fd, payload.data(), payload.size())) < 0)
        PERROR_EXIT("write");

    std::fstream cli = sfifo_fstream(pid);
    std::string char_count;
    cli >> char_count;
    std::cout << "Char count: " << char_count << std::endl;
    cli.close();
    close(srv_fd);
    return 0;
}
