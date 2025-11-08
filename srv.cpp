#include "sfifo.h"

#include <poll.h>

#include <string>
#include <fstream>
#include <iostream>
#include <csignal>

volatile sig_atomic_t interrupted = 0;
static int srv_fd = -1;

constexpr int PID_MAX {4194304};
const int PID_MAX_SLEN = std::to_string(PID_MAX).length();
const std::filesystem::path srv_fn = "srv";

void cleanup(int sig)
{
    interrupted = 1;
    std::string sig_name = (sig == SIGINT) ? "SIGINT" : "SIGTERM";
    int exit_code = (sig == SIGINT) ? 0 : 1;
    std::cout << "\nCaught " << sig_name << ", performing cleanup and exiting...\n";
    if (srv_fd != -1)
        close(srv_fd);

    if (!std::filesystem::remove(PATH_ROOT / srv_fn))
        perror("remove");
    exit(exit_code);
}

int main()
{
    std::error_code ec;
    std::filesystem::create_directory(PATH_ROOT, ec);
    if (ec && !std::filesystem::exists(PATH_ROOT))
        PERROR_EXIT("create_directory");
    std::signal(SIGINT, cleanup);
    std::signal(SIGTERM, cleanup);

    sfifo_mkfifo(PATH_ROOT, srv_fn);
    srv_fd = sfifo_open(PATH_ROOT, srv_fn);
    if (srv_fd == -1)
        PERROR_EXIT("open");
    FILE* fp = fdopen(srv_fd, "r");

    struct pollfd pfd;
    pfd.fd = srv_fd;
    pfd.events = POLLIN;

    while (1) {
        bool erratic_write_detected = false;

        std::cout << "Listening...\n";
        int ret = poll(&pfd, 1, -1);
        if (ret == -1) {
            perror("poll");
            exit(1);
        }

        int c = 0, cc = 0;
        std::string pid;
        while ((c = getc(fp)) != '\0' && !erratic_write_detected) {
            pid.append(1, c);
            if (++cc > PID_MAX_SLEN)
                erratic_write_detected = true;
        }
        if (erratic_write_detected) {
            std::cout << "Ingoring erratic write - PID to long...\n";
            while (getc(fp) != EOF)
                ;
            continue;
        }
        std::cout << "Handling client at " << PATH_ROOT << "/" << pid << "...\n";

        cc = 0;
        while ((c = getc(fp)) != '\0' && !erratic_write_detected) {
            if (++cc > ARG_MAX)
                erratic_write_detected = true;
        }
        if (erratic_write_detected) {
            std::cout << "Ingoring erratic write - message to long...\n";
            while (getc(fp) != EOF)
                ;
            continue;
        }

        std::fstream cli = sfifo_fstream(PATH_ROOT, pid);
        cli << cc << std::endl;
        cli.close();
        std::cout << "Handled client at " << PATH_ROOT << "/" << pid << std::endl;
    }
}
