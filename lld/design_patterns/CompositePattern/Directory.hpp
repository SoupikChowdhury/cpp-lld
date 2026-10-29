#pragma once
#include "FileSystem.hpp"
#include <vector>
#include <memory>
#include <algorithm>

/**
 * Composite Pattern - Composite
 * 
 * Represents a directory (can contain files and directories).
 */
class Directory : public FileSystem {
private:
    std::string directoryName;
    std::vector<std::shared_ptr<FileSystem>> children;

public:
    explicit Directory(const std::string& name) : directoryName(name) {}

    void add(std::shared_ptr<FileSystem> component) {
        children.push_back(component);
    }

    void remove(std::shared_ptr<FileSystem> component) {
        children.erase(
            std::remove(children.begin(), children.end(), component),
            children.end()
        );
    }

    void ls() override {
        std::cout << "Directory: " << directoryName << std::endl;
        for (const auto& child : children) {
            child->ls();
        }
    }
};
