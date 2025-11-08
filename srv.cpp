#include "sfifo.h"

#include <poll.h>

#include <string>
#include <fstream>
#include <iostream>
#include <csignal>
#include <cmath>

volatile sig_atomic_t interrupted = 0;
static int srv_fd = -1;

constexpr int PID_MAX {4194304};
const int PID_MAX_SLEN = (int)log10(abs(PID_MAX)) + 1;
const std::filesystem::path srv_fn = "srv";

void cleanup(int sig)
{
    interrupted = 1;
    std::string sig_name = (sig == SIGINT) ? "SIGINT" : "SIGTERM";
    int exit_code = (sig == SIGINT) ? 0 : 1;
    std::cout << "Caught " << sig_name << ", performing cleanup and exiting...";
    if (srv_fd != -1)
        close(srv_fd);

    if (!std::filesystem::remove(PATH_ROOT / srv_fn))
        perror("remove");
    exit(exit_code);
}

int main()
{
    int d = 0;
    std::error_code ec;
    std::filesystem::create_directory(PATH_ROOT, ec);
    if (ec && !std::filesystem::exists(PATH_ROOT)) {
        std::cout << "Error creating " << PATH_ROOT << ec.message() << '\n';
        exit(1);
    }
    std::signal(SIGINT, cleanup);
    std::signal(SIGTERM, cleanup);

    sfifo_mkfifo(PATH_ROOT, srv_fn);
    srv_fd = sfifo_open(PATH_ROOT, srv_fn);
    if (srv_fd == -1) {
        perror("open");
        exit(1);
    }
    FILE* fp = fdopen(srv_fd, "r");

    struct pollfd pfd;
    pfd.fd = srv_fd;
    pfd.events = POLLIN;

    while (!interrupted) {
        std::cout << ++d << std::endl;
        bool erratic_write_detected = false;
        int ret = poll(&pfd, 1, -1);
        if (ret == -1) {
            perror("poll");
            exit(1);
        }

        int c = 0, cc = 0;
        std::string pid;
        do {
            pid.append(1, c);
            if (++cc >= PID_MAX_SLEN) {
                std::cout << "Ingoring erratic write - PID to long..." << std::endl;
                erratic_write_detected = true;
            }
        } while ((c = getc(fp)) != '\0' && !interrupted && !erratic_write_detected);
        if (erratic_write_detected)
            continue;

        cc = 0;
        while ((c = getc(fp)) != '\0' && !interrupted && !erratic_write_detected) {
            if (++cc > ARG_MAX) {
                std::cout << "Ingoring erratic write - message to long..." << std::endl;
                erratic_write_detected = true;
            }
        }
        if (erratic_write_detected)
            continue;

        std::fstream cli = sfifo_fstream(PATH_ROOT, pid, 1);
        std::cout << ++d << std::endl;
        cli << cc << std::endl;
        cli.close();
    }
}
