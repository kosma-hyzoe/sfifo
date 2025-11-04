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
    int srv_fd;
    std::string pid;
    std::filesystem::path path, root, tmp_filename;
    std::fstream cli;
    struct sigaction sa{};

    sa.sa_handler = irq_handler;
    // sigaction(SIGINT, &sa, nullptr);
    // sigaction(SIGTERM, &sa, nullptr);

    // TODO: err handling
    std::filesystem::create_directory(PATH_ROOT);

    sfifo_mkfifo(PATH_ROOT, "srv");
    srv_fd = sfifo_open(PATH_ROOT, "srv");
    if (srv_fd == -1) {
        perror("open");
        exit(1);
    }
    FILE* fp = fdopen(srv_fd, "r");

    while (1) {
        // TODO: replace with poll
        while((c = getc(fp)) == -1){
            std::cout << int(c) << std::endl << "0" << std::endl;
            usleep(10000);
        }
        do {
            pid.append(1, c);
        } while ((c = getc(fp)) != '\0');

        char_count = 0;
        while ((c = getc(fp)) != '\0')
                char_count++;

        sfifo_mkfifo(PATH_ROOT, pid);
        cli = sfifo_fstream(PATH_ROOT, pid);
        cli << (char_count - 1) << std::endl;
        cli.close();
    }
    close(srv_fd);
}
