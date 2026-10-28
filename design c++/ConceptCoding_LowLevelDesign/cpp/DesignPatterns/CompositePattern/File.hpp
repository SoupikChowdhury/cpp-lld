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

    void ls() override {
        std::cout << "File: " << fileName << std::endl;
    }
};
