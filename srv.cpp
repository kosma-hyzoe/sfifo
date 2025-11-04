#include "common.h"

#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <csignal>


void dbg(std::string f)
{
    std::cout << f << std::endl;
}
int main()
{
    char c, tmp;
    int char_count;
    int srv_fd;
    FILE* fp;
    std::string pid;
    std::filesystem::path path, root, tmp_filename;
    std::fstream cli;
    struct sigaction sa{};

    // TODO: err handling
    std::filesystem::create_directory(PATH_ROOT);

    sfifo_mkfifo(PATH_ROOT, "srv");
    srv_fd = sfifo_open(PATH_ROOT, "srv");
    if (srv_fd == -1) {
        perror("open");
        exit(1);
    }
     fp = fdopen(srv_fd, "r");

    c = 0;
    while (1) {
        // TODO: replace with poll
        while((c = getc(fp)) == -1){
            usleep(10000);
        }
        do {
            pid.append(1, c);
        } while ((c = getc(fp)) != '\0');
        std::cout << pid << std::endl;

        char_count = 0;
        while ((c = getc(fp)) != '\0')
                char_count++;
        std::cout << char_count;
        sfifo_mkfifo(PATH_ROOT, pid);
        cli = sfifo_fstream(PATH_ROOT, pid);
        cli << (char_count - 1) << std::endl;
        cli.close();
    }
    close(srv_fd);
}
