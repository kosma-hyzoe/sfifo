#include "common.h"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

std::fstream get_fifo(std::filesystem::path target_dir,
                              std::filesystem::path filename) {
    if (!std::filesystem::exists(target_dir))
        std::filesystem::create_directory(target_dir);

    std::filesystem::path path = target_dir / filename;
    if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path \
                << " exists and is NOT a FIFO pipe...";
            exit(1);
        }
    } else if (mkfifo(path.c_str(), 0660) == -1) {
        perror("mkfifo");
        exit(1);
    }
    std::fstream fifo(path.c_str());
    return fifo;
}

