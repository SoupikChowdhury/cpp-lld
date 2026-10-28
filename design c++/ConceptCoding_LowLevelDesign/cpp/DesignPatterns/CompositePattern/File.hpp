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
    explicit File(const std::string&amp; name) : fileName(name) {}

    void ls() override {
        std::cout &lt;&lt; "File: " &lt;&lt; fileName &lt;&lt; std::endl;
    }
};
