#include "File.hpp"
#include "Directory.hpp"
#include &lt;memory&gt;

/**
 * Composite Pattern Demo
 * 
 * Creates a tree structure of files and directories.
 */
int main() {
    // Create movie directory
    auto movieDirectory = std::make_shared&lt;Directory&gt;("Movies");

    // Add a file directly to movies
    auto border = std::make_shared&lt;File&gt;("Border.mp4");
    movieDirectory-&gt;add(border);

    // Create subdirectory for comedy movies
    auto comedyMovieDirectory = std::make_shared&lt;Directory&gt;("Comedy Movies");
    auto hulchul = std::make_shared&lt;File&gt;("Hulchul.mp4");
    auto hera_pheri = std::make_shared&lt;File&gt;("Hera_Pheri.mp4");
    comedyMovieDirectory-&gt;add(hulchul);
    comedyMovieDirectory-&gt;add(hera_pheri);

    // Add comedy directory to movies
    movieDirectory-&gt;add(comedyMovieDirectory);

    // Create action subdirectory
    auto actionDirectory = std::make_shared&lt;Directory&gt;("Action Movies");
    auto dhoom = std::make_shared&lt;File&gt;("Dhoom.mp4");
    actionDirectory-&gt;add(dhoom);
    movieDirectory-&gt;add(actionDirectory);

    // List all - treats composite and leaf uniformly
    std::cout &lt;&lt; "=== File System Structure ===" &lt;&lt; std::endl;
    movieDirectory-&gt;ls();

    return 0;
}
