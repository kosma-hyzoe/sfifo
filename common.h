#include <filesystem>

#define PATH_ROOT "/tmp/sfifo"

std::fstream get_fifo(std::filesystem::path target_dir,
                       std::filesystem::path filename, bool create);

