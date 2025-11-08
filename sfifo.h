#include <filesystem>
#include <string>


#define PERROR_EXIT(func) \
    do { \
        perror(func); \
        exit(1); \
    } while(0)

const std::string PATH_ROOT = "/tmp/sfifo";
constexpr int ARG_MAX = 2097152;

int sfifo_mkfifo(std::string, std::string);
std::fstream sfifo_fstream(std::string, std::string);
int sfifo_open(std::string, std::string);
