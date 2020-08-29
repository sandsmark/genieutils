#include "genie/lang/LangFile.h"
#include "genie/util/Utility.h"
#include "genie/util/Logger.h"


#include <iostream>
#include <filesystem>

#define BLOCK_SIZE 4096 // lol what do I know

void printUsage(const char *appName)
{
    std::cerr << "\tUsage: " << appName << " list|show gamedir [string-id]" << std::endl;
    std::cerr << "\tDat file is used to get the filenames" << std::endl;
}

int main(int argc, char *argv[]) try
{
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    const std::string command = argv[1];

    std::string gamePath = argv[2];

    std::vector<std::string> dlls{
        "language.dll",
        "language_x1.dll",
        "language_x1_p1.dll"
    };

    int id = -1;
    if (argc > 3) {
        id = std::stoi(argv[3]);
    }

    for (const std::string &dll : dlls) {
        const std::string filepath = gamePath + "/" + dll;
        if (!std::filesystem::exists(filepath)) {
            continue;
        }
        try {
            genie::LangFile langfile;
            langfile.load(filepath);
            if (id >= 0) {
                const std::string str = langfile.getString(id);
                if (!str.empty()) {
                    std::cout << id << '\t' << dll << "\t'" << str << "'" << std::endl;
                }
                continue;
            } else {
                std::cout << "Listing all " << langfile.allIds().size() << std::endl;
                for (const uint32_t id : langfile.allIds()) {
                    const std::string str = langfile.getString(id + 0);
                    std::cout << id << '\t' << dll << "\t'" << str << "'" << std::endl;
                }
            }
        } catch (const std::exception &error) {
            std::cerr << error.what() << std::endl;
        }
    }



    return 0;
} catch (const std::exception &e) { // just in case I forget to catch something, I hate exceptions
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
}

