#include "common.h"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

std::fstream get_fifo(std::filesystem::path target_dir,
                              std::filesystem::path filename, bool create) {
    if (!std::filesystem::exists(target_dir))
        std::filesystem::create_directory(target_dir);

    std::filesystem::path path = target_dir / filename;
    if (create) {
        if  (mkfifo(path.c_str(), 0660) == -1) {
            // TODO: what if exists?
            perror("mkfifo");
            exit(1);
        }
    } else if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            std::cout << "ERROR: File " << path \
            << " exists and is NOT a FIFO pipe" << std::endl;
            exit(1);
        }
    } else {
            std::cout << "ERROR: File " << path << " does not exist";
            exit(1);
    }

    std::fstream fifo(path.c_str());
    return fifo;
}

