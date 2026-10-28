#pragma once
#include "FileSystem.hpp"
#include &lt;vector&gt;
#include &lt;memory&gt;

/**
 * Composite Pattern - Composite
 * 
 * Represents a directory (can contain files and directories).
 */
class Directory : public FileSystem {
private:
    std::string directoryName;
    std::vector&lt;std::shared_ptr&lt;FileSystem&gt;&gt; children;

public:
    explicit Directory(const std::string&amp; name) : directoryName(name) {}

    void add(std::shared_ptr&lt;FileSystem&gt; component) {
        children.push_back(component);
    }

    void remove(std::shared_ptr&lt;FileSystem&gt; component) {
        children.erase(
            std::remove(children.begin(), children.end(), component),
            children.end()
        );
    }

    void ls() override {
        std::cout &lt;&lt; "Directory: " &lt;&lt; directoryName &lt;&lt; std::endl;
        for (const auto&amp; child : children) {
            child-&gt;ls();
        }
    }
};
