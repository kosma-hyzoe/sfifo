#include <filesystem>
#include <string>

#define PATH_ROOT "/tmp/sfifo"

int sfifo_mkfifo(std::string, std::string);
std::fstream sfifo_fstream(std::string, std::string);
int sfifo_open(std::string, std::string);
