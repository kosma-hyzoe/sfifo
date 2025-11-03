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
    std::filesystem::path path, root, tmp_filename;
    std::string line, msg;
    std::string n_of_chars;

    root = PATH_ROOT;
    std::fstream srv = get_fifo(root, "srv", false);
    pid = std::to_string(getpid());
    std::fstream response = get_fifo(PATH_ROOT, pid, true);


    std::cout << ">> Enter your message, one or more lines separated by ENTER. " \
        << "Press CTRL+D when done" << std::endl;
    while (getline(std::cin, line))
        msg.append(line).append("\n");
    srv <<  pid.string() << std::endl << msg << std::endl;
    std::cout << ">> Sent!" << std::endl;

   while (response.peek() == std::ifstream::traits_type::eof())
        usleep(1000);

    // Read data from FIFO
    if (std::getline(response, n_of_chars)) {
        std::cout << ">> Data received: " << n_of_chars << std::endl;
    } else {
        std::cout << ">> Failed to read data from FIFO." << std::endl;
    }

    if (std::remove((root / pid).c_str()))
        std::cout << ":V" << std::endl;
    return 0;
}
