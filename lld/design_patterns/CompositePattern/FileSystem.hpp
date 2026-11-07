#pragma once
#include <iostream>
#include <string>

/**
 * Composite Pattern - Component Interface
 * 
 * Common interface for both File and Directory.
 */
class FileSystem {
public:
    virtual ~FileSystem() = default;
    virtual void ls(int depth = 0) = 0;

    // Is this component, or anything inside it, the given one?
    virtual bool contains(const FileSystem* other) const { return this == other; }
};
