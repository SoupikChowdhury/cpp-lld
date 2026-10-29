#include "File.hpp"
#include "Directory.hpp"
#include <memory>

/**
 * Composite Pattern Demo
 * 
 * Creates a tree structure of files and directories.
 */
int main() {
    // Create movie directory
    auto movieDirectory = std::make_shared<Directory>("Movies");

    // Add a file directly to movies
    auto border = std::make_shared<File>("Border.mp4");
    movieDirectory->add(border);

    // Create subdirectory for comedy movies
    auto comedyMovieDirectory = std::make_shared<Directory>("Comedy Movies");
    auto hulchul = std::make_shared<File>("Hulchul.mp4");
    auto hera_pheri = std::make_shared<File>("Hera_Pheri.mp4");
    comedyMovieDirectory->add(hulchul);
    comedyMovieDirectory->add(hera_pheri);

    // Add comedy directory to movies
    movieDirectory->add(comedyMovieDirectory);

    // Create action subdirectory
    auto actionDirectory = std::make_shared<Directory>("Action Movies");
    auto dhoom = std::make_shared<File>("Dhoom.mp4");
    actionDirectory->add(dhoom);
    movieDirectory->add(actionDirectory);

    // List all - treats composite and leaf uniformly
    std::cout << "=== File System Structure ===" << std::endl;
    movieDirectory->ls();

    return 0;
}
