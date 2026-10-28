#pragma once
#include &lt;iostream&gt;
#include &lt;string&gt;

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
