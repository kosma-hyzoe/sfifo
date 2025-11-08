#include "sfifo.h"

#include <poll.h>
#include <fcntl.h>

#include <string>
#include <fstream>
#include <iostream>
#include <csignal>

volatile sig_atomic_t interrupted = 0;

static int srv_fd = -1;
const int PID_MAX_LEN = std::to_string(get_pid_max()).length();

void cleanup(int sig)
{
    interrupted = sig;
    std::cout << "\nCaught SIGINT, performing cleanup and exiting...\n";
    if (srv_fd != -1)
        close(srv_fd);

    if (!std::filesystem::remove(SRV_PATH) || !std::filesystem::remove(PATH_ROOT))
        perror("remove");
    exit(0);
}

int main()
{
    std::error_code ec;
    std::filesystem::create_directory(PATH_ROOT, ec);
    if (ec && !std::filesystem::exists(PATH_ROOT))
        PERROR_EXIT("create_directory");
    std::signal(SIGINT, cleanup);

    sfifo_mkfifo(SRV_PATH);
    srv_fd = sfifo_open(SRV_PATH, O_RDWR);
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
            if (++cc > PID_MAX_LEN)
                erratic_write_detected = true;
        }
        if (erratic_write_detected) {
            std::cout << "Ingoring erratic write - PID too long...\n";
            while (getc(fp) != EOF)
                ;
            continue;
        }
        std::string cli_filename = get_cli_filename(pid);
        std::cout << "Handling client at " << PATH_ROOT << "/" << cli_filename << "...\n";

        cc = 0;
        while ((c = getc(fp)) != '\0' && !erratic_write_detected) {
            if (++cc > ARG_MAX)
                erratic_write_detected = true;
        }
        if (erratic_write_detected) {
            std::cout << "Ingoring erratic write - message too long...\n";
            while (getc(fp) != EOF)
                ;
            continue;
        }

        std::fstream cli = sfifo_fstream(get_cli_filename(pid));
        cli << cc << std::endl;
        cli.close();
        std::cout << "Handled client at " << PATH_ROOT << "/" << cli_filename << std::endl;
    }
}
