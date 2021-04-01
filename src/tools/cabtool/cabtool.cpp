#include "genie/file/CabFile.h"

#include <filesystem>

using namespace genie;

void printUsage(const char *appName)
{
    printf("Usage: %s list|show|extract filename.cab\n", appName);
}
int main(int argc, char *argv[]) try
{
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }
    const std::string cabFilename = argv[2];
    if (!std::filesystem::exists(cabFilename)) {
        printUsage(argv[0]);
        return 1;
    }

    CabFile file;
    file.setVerboseMode(true);
    file.load(cabFilename);

    const std::string operation = argv[1];
    if (operation == "list") {
        for (const std::string &filename : file.filenames()) {
            std::cout << filename << std::endl;
        }
    } else if (operation == "show") {
        if (argc < 4) {
            printUsage(argv[0]);
            return 1;
        }
        file.readFile(argv[3]);
    }

    return 0;
} catch (const std::exception &e) {
    std::cerr << "uncatched exception '" << e.what() << "', terminating uncleanly" << std::endl;
    return 1;
}


