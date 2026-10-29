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
    virtual void ls() = 0;
};
