#include "common.h"

#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> interrupted(false);
void irq_handler(int)
{
    interrupted = true;
}

int main()
{
    char c, tmp;
    int char_count;
    std::string pid;
    std::filesystem::path path, root, tmp_filename;
    std::fstream cli, srv;
    struct sigaction sa{};

    sa.sa_handler = irq_handler;
    // sigaction(SIGINT, &sa, nullptr);
    // sigaction(SIGTERM, &sa, nullptr);

    // TODO: err handling
    std::filesystem::create_directory(PATH_ROOT);

    sfifo_mkfifo(PATH_ROOT, "srv");
    srv = sfifo_fstream(PATH_ROOT, "srv");

    c = -1;
    while (1) {
        // TODO: replace with poll
        srv.get(c);
        std::cout << c;
        do {
            pid.append(1, c);
        } while (srv.get(c) && c != '\0');

        char_count = 0;
        while (srv.get(c) && c != '\0')
                char_count++;

        sfifo_mkfifo(PATH_ROOT, pid);
        cli = sfifo_fstream(PATH_ROOT, pid);
        cli << (char_count - 1) << std::endl;
        cli.close();
    }
    srv.close();
}
