#include <filesystem>
#include <unistd.h>
#include <iostream>

#define PERROR_EXIT(func) \
    do { \
        perror(func); \
        exit(1); \
    } while(0)
#define PERROR_EXIT_RET(func, ret) \
    do { \
        perror(func); \
        exit((ret)); \
    } while(0)

const std::string PATH_ROOT = "/tmp/sfifo";
const std::string SRV_PATH = PATH_ROOT + "/srv";

const long ARG_MAX = {sysconf(_SC_ARG_MAX)};

int sfifo_mkfifo(std::string);
std::fstream sfifo_fstream(std::string);
int sfifo_open(std::string, int);
int get_pid_max();

