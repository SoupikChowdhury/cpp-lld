#pragma once
#include "FileSystem.hpp"

/**
 * Composite Pattern - Leaf
 * 
 * Represents a file (no children).
 */
class File : public FileSystem {
private:
    std::string fileName;

public:
    explicit File(const std::string& name) : fileName(name) {}

    void ls(int depth = 0) override {
        std::cout << std::string(depth * 2, ' ') << "File: " << fileName << std::endl;
    }
};
