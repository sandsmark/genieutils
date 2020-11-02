#include "genie/resource/DrsFile.h"
#include "genie/dat/DatFile.h"
#include "genie/util/Utility.h"

#include <iostream>
#include <filesystem>

#define BLOCK_SIZE 4096 // lol what do I know

void printUsage(const char *appName)
{
    std::cerr << "\tUsage: " << appName << " [datfile] list|extract <file.drs> [filetoextract]" << std::endl;
    std::cerr << "\tDat file is used to get the filenames" << std::endl;
}

void printByteSize(float size)
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

    std::cout << std::round(size) << ' ' << suffix;
}


int main(int argc, char *argv[]) try
{
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    const std::string command = (argc == 3 || argc == 4) ? argv[1] : argv[2];
    std::string drsFile;
    std::string datFile;
    std::cout << argc << std::endl;
    for (int i=0; i< argc; i++) std::cout << i << argv[i] << std::endl;

    std::string toExtract;

    if (command == "extract") {
        if (argc != 4 && argc != 5) {
            std::cout << "not enough arguments for extract" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        toExtract = argc > 4 ? argv[4] : argv[3];
        drsFile = argc > 4 ? argv[3] : argv[2];
        datFile = argc > 4 ? argv[2] : "";

        if (!std::filesystem::exists(drsFile)) {
            printUsage(argv[0]);
            return 1;
        }
    } else if (command == "list") {
        datFile = argc > 3 ? argv[1] : "";
        drsFile = argc > 3 ? argv[3] : argv[2];
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    if (!datFile.empty() && !std::filesystem::exists(datFile)) {
        std::cout << "Dat file does not exist: " << datFile << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    if (!std::filesystem::exists(drsFile)) {
        std::cout << "drs file " << drsFile << " does not exist" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    genie::GameVersion gameVersion = genie::GV_Tapsa; // will try to guess frmo file contents
    if (datFile.empty()) {
        std::string dataPath = std::filesystem::path(drsFile).parent_path().string() + '/';
        std::cout << "Looking for dat file in " << dataPath << std::endl;

        std::vector<std::pair<std::string, genie::GameVersion>> datFilenames({
            {"empires2_x1_p1.dat", genie::GV_TC  }, // the conquerors, patch 1
            {"empires2_x1.dat",    genie::GV_TC  }, // the conquerors
            {"empires2.dat",       genie::GV_AoK }, // age of kings
            {"empires_x1.dat",     genie::GV_RoR },
            {"Empires.dat",        genie::GV_AoE },
            {"GENIE.DAT",          genie::GV_SWGB },
        });
        for (const std::pair<std::string, genie::GameVersion> &datfile : datFilenames) {
            std::string potential = dataPath + datfile.first;
            if (std::filesystem::exists(potential)) {
                datFile = potential;
                gameVersion = datfile.second;
                break;
            }
        }
    }

    std::cout << "Using datfile " << datFile << " to " << command << " from " << drsFile << std::endl;

    genie::Logger::setLogLevel(genie::Logger::L_DEBUG);

    genie::DatFile dat;
    bool hasDat = false;
    if (!datFile.empty()) try {
        dat.setGameVersion(gameVersion);
        dat.load(datFile);
        hasDat = true;
    } catch (const std::exception &e) {
        std::cerr << "Failed to load dat file: " << e.what() << std::endl;
        return 1;
    }


    genie::DrsFile drs;
    drs.setGameVersion(gameVersion);
    try {
        drs.load(drsFile);
    } catch (const std::exception &e) {
        std::cerr << "Failed to load DRS: " << e.what() << std::endl;
        return 1;
    }

    if (toExtract.empty()) {
        for (const uint32_t id : drs.allIds()) {
            std::cout << id << '\t';

            const std::string type = drs.idType(id);
            if (hasDat) {
                const std::string filename = dat.resourceFilename(id);
                std::cout << filename;

                if (!std::filesystem::path(filename).has_extension()) {
                    std::cout << '.' << type;
                }
                std::cout << '\t';
            } else {
                std::cout << id << '.' << type << "\t";
            }

            printByteSize(drs.fileSize(id));

            if (type == "wav") {
                std::cout << '\t';
                genie::WavFilePtr wav = drs.getWavFile(id);
                if (!wav || !wav->isValid()) {
                    std::cout << "INVALID" << std::endl;
                    continue;
                }

                const int channels = wav->header().NumChannels;
                std::cout << wav->header().formatName() << ','
                    << " " << channels << (channels > 1 ? " channels, " : " channel, ")
                    << "samplerate: " << wav->header().SampleRate << " Hz";
            }

            std::cout << std::endl;
        }
        return 0;
    }

    std::vector<std::string> ids;

    if (genie::util::stringStartsWith(toExtract, "*.")) {
        std::string type = toExtract.substr(2);
        for (const uint32_t id : drs.allIds()) {
            if (drs.idType(id) == type) {
                // hacky and inefficient so sue me
                ids.push_back(std::to_string(id));
            }
        }
    } else {
        ids.push_back(toExtract);
    }

    for (const std::string &toExtract : ids) {
        std::string outFilename = toExtract;

        // meh, not efficient but meh
        int id = atoi(toExtract.c_str());
        if (!id) {
            if (!hasDat) {
                std::cerr << "invalid id and no dat file" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
            const std::string toExtractNoExt = std::filesystem::path(toExtract).stem();
            for (const std::pair<uint32_t, std::string> p : dat.resourceFilenames()) {
                if (p.second == toExtract || p.second == toExtractNoExt) {
                    id = p.first;
                    break;
                }
            }
            if (!id) {
                std::cerr << "Failed to find " << toExtract << std::endl;
                printUsage(argv[0]);
                return 1;
            }
            std::cout << "Extracting ID " << id << std::endl;
        } else {
            outFilename = dat.resourceFilename(id);
        }

        if (!std::filesystem::path(outFilename).has_extension()) {
            outFilename += '.' + drs.idType(id);
        }

        std::cout << "Extracting " << toExtract << " to " << outFilename << std::endl;

        if (outFilename.empty()) {
            std::cerr << "Failed to find filename for " << id << std::endl;
            outFilename = toExtract + ".extracted";
        }

        if (std::filesystem::exists(outFilename)) {
            std::cout << outFilename << " exists, overwrite? [y/n] " << std::endl;
            char response = std::cin.get();

            if (response != 'y') {
                std::cout << "Aborting" << std::endl;
                return 1;
            }
        }

        const ssize_t size = drs.fileSize(id);
        const std::ios::pos_type offset = drs.fileOffset(id);
        if (size <= 0 || offset <= 0) {
            std::cerr << "Invalid file size " << size << " or offset " << offset << std::endl;
            return 1;
        }

        std::ifstream source(drsFile, std::ios_base::binary | std::ios_base::in);
        if (!source.good()) {
            std::cerr << "Failed to open " << drsFile << " for reading" << std::endl;
            return 1;
        }

        source.seekg(offset);
        if (!source.good()) {
            std::cout << "Seek to " << offset << " failed" << std::endl;
            return 1;
        }

        std::ofstream target(outFilename, std::ios_base::binary | std::ios_base::out);
        if (!target.good()) {
            std::cerr << "Failed to open " << outFilename << " for writing" << std::endl;
            return 1;
        }

        std::copy_n(std::istreambuf_iterator<char>(source), size, std::ostreambuf_iterator<char>(target));

        if (!source.good()) {
            std::cout << "Reading failed" << std::endl;
            return 1;
        }
        if (!target.good()) {
            std::cout << "Writing to " << outFilename << "  failed" << std::endl;
            return 1;
        }

        source.close();
        target.close();
    }


    return 0;
} catch (const std::exception &e) { // just in case I forget to catch something, I hate exceptions
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
}

