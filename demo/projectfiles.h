#pragma once

#include <string>
#include <experimental/filesystem>

namespace projectfiles {

static const std::string root_file(".ROVERSIGHT_ROOT_DIR");

static std::string rootdir() {
    namespace fs = std::experimental::filesystem;

    std::string rootdir("");
    for(const auto& p : fs::recursive_directory_iterator("../..")) {
        if (p.path().filename() == root_file) {
            rootdir = fs::canonical(p).remove_filename();
            break;
        }
    }

    // If still not found, throw an exception.
    if(rootdir.empty()) {
        std::string msg("Failed to find project root directory. Did you delete " + root_file + "?");
        throw std::runtime_error(msg);
    }
    return rootdir;
}

static std::string get(const std::string& filename) {
    namespace fs = std::experimental::filesystem;

    const std::string root = rootdir();
    const fs::path filepath = fs::path(root) / fs::path(filename);
    if(!fs::exists(filepath)) {
        const std::string msg ("Failed to find file " + filename + ".\n");
        throw std::runtime_error(msg);
    }
    return filepath;
}

}
