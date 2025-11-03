#include <filesystem>
#include <string>

#define PATH_ROOT "/tmp/sfifo"

std::fstream sfifo_fstream(std::filesystem::path);
int sfifo_open(std::string);
