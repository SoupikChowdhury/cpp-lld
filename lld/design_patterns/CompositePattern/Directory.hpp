#pragma once
#include "FileSystem.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

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
        if (!component) {
            throw std::invalid_argument("Cannot add nothing to " + directoryName);
        }
        // A directory inside itself would make ls recurse forever and leak the shared_ptrs
        if (component->contains(this)) {
            throw std::invalid_argument("Cannot put " + directoryName + " inside itself");
        }
        if (std::find(children.begin(), children.end(), component) != children.end()) {
            throw std::invalid_argument("Already in " + directoryName);
        }
        children.push_back(component);
    }

    void remove(std::shared_ptr<FileSystem> component) {
        children.erase(
            std::remove(children.begin(), children.end(), component),
            children.end()
        );
    }

    void ls(int depth = 0) override {
        std::cout << std::string(depth * 2, ' ') << "Directory: " << directoryName << std::endl;
        for (const auto& child : children) {
            child->ls(depth + 1);
        }
    }

    bool contains(const FileSystem* other) const override {
        if (this == other) return true;
        for (const auto& child : children) {
            if (child->contains(other)) return true;
        }
        return false;
    }
};
