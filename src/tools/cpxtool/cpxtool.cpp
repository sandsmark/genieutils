#include "genie/script/ScnFile.h"
#include "genie/dat/DatFile.h"
#include "genie/util/Utility.h"
#include "genie/util/Logger.h"


#include <iostream>
#include <filesystem>

#define BLOCK_SIZE 4096 // lol what do I know

void printUsage(const char *appName)
{
    std::cerr << "\tUsage: " << appName << " list[-verbose]|extract|show <file.cpx|cpn> [scnfile.scn] [filetoextract outpath]" << std::endl;
    std::cerr << "\tDat file is used to get the filenames" << std::endl;
}

std::string prettyBytes(float size)
{
    // ugly but easy to think about or something
    std::string suffix = "B";
    if (size > 1024) {
        size /= 1024;
        suffix = "KB";
    }
    if (size > 1024) {
        size /= 1024;
        suffix = "MB";
    }
    if (size > 1024) {
        size /= 1024;
        suffix = "GB";
    }
    if (size > 1024) {
        size /= 1024;
        suffix = "TB";
    }

    // holy fucking shit std, would it hurt to read a book on sane API design?
    return std::to_string(int(size + 0.5)) + ' ' + suffix;
}


int main(int argc, char *argv[]) try
{
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    const std::string command = argv[1];
    std::string cpxFile;

    std::string toExtract;

    cpxFile = argv[2];

    if (argc > 3) {
        toExtract = argv[3];
    }
    if (command == "extract") {
        if (argc < 4) {
            std::cout << "not enough arguments for extract" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        toExtract = argv[3];
    } else if (command == "list" || command == "list-verbose") {
        // todo check arg count
    } else if (command == "show") {
        // todo check arg count
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    if (!std::filesystem::exists(cpxFile)) {
        std::cout << "campaign file " << cpxFile << " does not exist" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    genie::Logger::setLogLevel(genie::Logger::L_DEBUG);

    genie::DatFile dat;


    genie::CpxFile campaign;
    if (command == "list-verbose") {
        campaign.verbose = true;
    }
    try {
        campaign.load(cpxFile);
    } catch (const std::exception &e) {
        std::cerr << "Failed to load DRS: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Name: " << campaign.name << std::endl;
    std::cout << "Version: " << campaign.version << std::endl;
    std::cout << "File count: " << campaign.getFilecount() << std::endl;

    if (toExtract.empty()) {
        size_t colWidth = 1;

        // Not the most efficient, so sue me
        for (const std::string &filename : campaign.getFilenames()) {
            colWidth = std::max(colWidth, filename.length());
            genie::CpxIncludedFile file = campaign.getRawFile(filename);
            colWidth = std::max(colWidth, file.filename.length());
        }

        // std is a fucking piece of shit
        // what would be so bad with some convenience functions?
        colWidth += 2;
        std::cout << std::right << std::setw(colWidth) << "Filename"
                  << std::setw(colWidth) << "Size"
                  << std::setw(colWidth) << "Offset"
                  << std::setw(colWidth) << "Identifier"
                  << std::setw(colWidth) << "Version"
                  << std::setw(colWidth) << "Bundled files"
                  << std::endl;

        for (const std::string &filename : campaign.getFilenames()) {
            std::cout << filename << '\t';

            genie::CpxIncludedFile file = campaign.getRawFile(filename);
            std::cout << std::right << std::setw(colWidth) << prettyBytes(file.size)
             << std::setw(colWidth) << std::to_string(file.offset)
             << std::setw(colWidth) <<  file.identifier
                ;

            try {
                genie::ScnFilePtr scenario = file.getScnFile();
                std::cout << std::setw(colWidth) <<  scenario->version
                          << std::setw(colWidth) << scenario->getFileCount()
                          << std::endl;
                          ;
            } catch (const std::exception &e) {
                std::cout << "\tInvalid (" << e.what() << ")" << std::endl;
            }

        }
        return 0;
    }

//    std::cout << "Opened scenario" << std::endl;
    if ((command == "list" || command == "list-verbose") && argc == 4) {
        genie::ScnFilePtr scenario = campaign.getScnFile(toExtract);
        if (!scenario) {
            std::cerr << "Failed to open" << toExtract;
            return 1;
        }
        for (const genie::ScnPersonalityScript &script : scenario->includedFiles)  {
            std::cout << prettyBytes(script.content.length()) << "\t'" << script.filename << "'" << std::endl;
        }
        return 0;
    }

    if (command == "show" && argc > 4) {
        genie::ScnFilePtr scenario = campaign.getScnFile(toExtract);
        if (!scenario) {
            std::cerr << "Failed to open" << toExtract;
            return 1;
        }
         for (const genie::ScnPersonalityScript &script : scenario->includedFiles)  {
             std::cout << "------" << std::endl;
             if (script.filename != argv[4]) {
                 continue;
             }
             std::cout << script.content << std::endl;
             std::cout << "------" << std::endl;
             return 0;
         }
         std::cerr << "Failed to find " << argv[4] << std::endl;
         return 1;
    }
    if (command == "extract" && argc == 5) {
        std::cout << "Extracting " << toExtract << " to " << argv[4] << std::endl;
        genie::CpxIncludedFile file = campaign.getRawFile(toExtract);
        file.extractTo(argv[4]);
        std::cout << "Done" << std::endl;
        return 0;
    }


    return 0;
} catch (const std::exception &e) { // just in case I forget to catch something, I hate exceptions
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
}

