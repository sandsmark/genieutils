#include "genie/savegame/MgxFile.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Please pass filename for an mgx file" << std::endl;
        return 1;
    }

    genie::MgxFile file;
    file.load(argv[1]);
    return 1;
}

