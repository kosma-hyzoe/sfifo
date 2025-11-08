#include <filesystem>
#include <string>

const std::string PATH_ROOT = "/tmp/sfifo";
constexpr int ARG_MAX = 2097152;

int sfifo_mkfifo(std::string, std::string);
std::fstream sfifo_fstream(std::string, std::string, int);
int sfifo_open(std::string, std::string);
