#include <iostream>

#include "genie/util/Logger.h"
#include "genie/dat/DatFile.h"
#include "genie/util/Utility.h"

#include "../src/tools/bincompare/bincomp.h"

#define BOOST_TEST_MODULE geniedat_torture_chamber
#include <boost/test/unit_test.hpp>

const char *const DAT_PATH = "tmp/";

std::string getFileName(genie::GameVersion gv)
{
    std::string file_name(DAT_PATH);

    switch (gv) {
    case genie::GV_AoE:
        file_name += "Empires.dat";
        break;

    case genie::GV_RoR:
        file_name += "empires_x1.dat";
        break;

    case genie::GV_AoK:
        file_name += "empires2.dat";
        break;

    case genie::GV_TC:
        file_name += "empires2_x1_p1.dat";
        break;

    case genie::GV_SWGB:
        file_name += "genie.dat";
        break;

    case genie::GV_CC:
        file_name += "genie_x1.dat";
        break;

    case genie::GV_None:
        break;
    default:
        BOOST_FAIL("Invalid game version: " + std::to_string(int(gv)));
        break;
    }

    const std::string resolved = genie::util::resolvePathCaseInsensitive(file_name, genie::util::executableDirectory());
    BOOST_WARN(!resolved.empty());

    return resolved;
}

std::unique_ptr<genie::DatFile> openFile(genie::GameVersion gv)
{
    const std::string filepath = getFileName(gv);
    if (filepath.empty()) {
        return nullptr;
    }
    std::unique_ptr<genie::DatFile> file = std::make_unique<genie::DatFile>();
    file->setFileName(filepath);
    file->setVerboseMode(true);
    file->setGameVersion(gv);
    file->load();
    return file;
}

void writeReadCheck(const genie::GameVersion gv)
{
    genie::Logger::setLogLevel(genie::Logger::L_INFO);
    std::cout << "Read write testing game version " << genie::DatFile::versionName(gv) << std::endl;

    genie::DatFile writeFile;
    writeFile.setVerboseMode(true);
    writeFile.setGameVersion(gv);

    writeFile.Civs.resize(1);
    writeFile.Civs[0].Units.resize(6);
    writeFile.Civs[0].UnitPointers.resize(6, 1);

    writeFile.Civs[0].Units[4].ID = 1337;
    writeFile.Civs[0].Units[4].Moving.TrackingUnit = 5;
    writeFile.Civs[0].Units[4].Creatable.TrainTime = 999;
    writeFile.Civs[0].Units[4].Creatable.ButtonID = 100;

    writeFile.Civs[0].Units[5].ID = 1338;
    writeFile.Civs[0].Units[5].Type = genie::Unit::CreatableType;
    writeFile.Civs[0].Units[5].Moving.TrackingUnit = 5;
    writeFile.Civs[0].Units[5].Creatable.TrainTime = 999;
    writeFile.Civs[0].Units[5].Creatable.ButtonID = 100;

    genie::Unit a = writeFile.Civs[0].Units[5];
    genie::Unit b(a);
    std::cout << " === Saving... ===" << std::endl;
    writeFile.saveAs("temp.dat");
    std::cout << "Size after saving " << writeFile.Civs.size() << std::endl;


    genie::DatFile readFile;
    readFile.setVerboseMode(true);
    readFile.setGameVersion(gv);
    std::cout << " === Loading... ===" << std::endl;
    readFile.load("temp.dat");

    BOOST_REQUIRE_GT(readFile.Civs.size(), 0);
    BOOST_REQUIRE_GT(readFile.Civs[0].Units.size(), 5);

    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[4].ID, 1337);
    // It should not get these, it is not the right type
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[4].Moving.TrackingUnit, -1);
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[4].Creatable.TrainTime, -1);
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[4].Creatable.ButtonID, -1);

    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[5].ID, 1338);
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[5].Moving.TrackingUnit, 5);
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[5].Creatable.TrainTime, 999);
    BOOST_CHECK_EQUAL(readFile.Civs[0].Units[5].Creatable.ButtonID, 100);

    BOOST_CHECK_EQUAL(a.Moving.TrackingUnit, 5);
    BOOST_CHECK_EQUAL(a.Creatable.TrainTime, 999);
    BOOST_CHECK_EQUAL(a.Creatable.ButtonID, 100);

    BOOST_CHECK_EQUAL(b.Moving.TrackingUnit, 5);
    BOOST_CHECK_EQUAL(b.Creatable.TrainTime, 999);
    BOOST_CHECK_EQUAL(b.Creatable.ButtonID, 100);

    // because of the 'invalid' type
    BOOST_CHECK(!readFile.compareTo(writeFile));

    // Now fix the type, and check that things are OK
    writeFile.Civs[0].Units[4].Type = genie::Unit::BuildingType;
    writeFile.saveAs("temp.dat");
    readFile.load("temp.dat");
    BOOST_CHECK(readFile.compareTo(writeFile));
}

BOOST_AUTO_TEST_CASE(write_read_test)
{
    BOOST_TEST_MESSAGE("Write then read all formats test");

    writeReadCheck(genie::GV_AoE);
    writeReadCheck(genie::GV_RoR);
    writeReadCheck(genie::GV_AoK);
    writeReadCheck(genie::GV_TC);
    writeReadCheck(genie::GV_SWGB);
    writeReadCheck(genie::GV_CC);
}

BOOST_AUTO_TEST_CASE(simple_change_values_test)
{
    std::cout << "Simple change values test" << std::endl;

    std::unique_ptr<genie::DatFile> file = openFile(genie::GV_TC);
    BOOST_REQUIRE(file);

    std::cout << file->Civs.size() << std::endl;
    BOOST_REQUIRE_GT(file->Civs.size(), 0);
    BOOST_REQUIRE_GT(file->Civs[0].Units.size(), 5);

    file->Civs[0].Units[4].Moving.TrackingUnit = 5;
    file->Civs[0].Units[4].Creatable.TrainTime = 999;
    file->Civs[0].Units[4].Creatable.ButtonID = 100;

    file->Civs[0].Units[5].Moving.TrackingUnit = 5;
    file->Civs[0].Units[5].Creatable.TrainTime = 999;
    file->Civs[0].Units[5].Creatable.ButtonID = 100;

    genie::Unit a = file->Civs[0].Units[5];
    std::cout << " === Saving... ===" << std::endl;
    file->saveAs("temp.dat");


    std::cout << " === Loading... ===" << std::endl;

    genie::DatFile readFile;
    readFile.setVerboseMode(true);
    readFile.setGameVersion(genie::GV_TC);
    readFile.load("temp.dat");

    BOOST_REQUIRE_GT(readFile.Civs.size(), 0);
    BOOST_REQUIRE_GT(readFile.Civs[0].Units.size(), 5);

    BOOST_CHECK( readFile.Civs[0].Units[4].Moving.TrackingUnit == 5 );
    BOOST_CHECK( readFile.Civs[0].Units[4].Creatable.TrainTime == 999 );
    BOOST_CHECK( readFile.Civs[0].Units[4].Creatable.ButtonID == 100 );

    BOOST_CHECK( readFile.Civs[0].Units[5].Moving.TrackingUnit == 5 );
    BOOST_CHECK( readFile.Civs[0].Units[5].Creatable.TrainTime == 999 );
    BOOST_CHECK( readFile.Civs[0].Units[5].Creatable.ButtonID == 100 );

    genie::Unit b = readFile.Civs[0].Units[5];

    BOOST_CHECK( b.Moving.TrackingUnit == 5 );
    BOOST_CHECK( b.Creatable.TrainTime == 999 );
    BOOST_CHECK( b.Creatable.ButtonID == 100 );

    BOOST_CHECK( a.Moving.TrackingUnit == 5 );
    BOOST_CHECK( a.Creatable.TrainTime == 999 );
    BOOST_CHECK( a.Creatable.ButtonID == 100 );
}

int readWriteDiff(genie::GameVersion gv)
{
    std::cout << "Read write diff testing version " << genie::DatFile::versionName(gv) << std::endl;
    genie::DatFile file;
    file.setGameVersion(gv);

    const std::string filepath = getFileName(gv);
    BOOST_WARN(!filepath.empty());
    if (filepath.empty()) {
        return 0;
    }

    genie::DatFile::extractRaw(filepath, filepath + ".raw_orig");
    std::cout << "Loading " << filepath << std::endl;
    file.load(filepath);

    file.saveAs(filepath + ".saved");

    {
        genie::DatFile reloaded;
        reloaded.setGameVersion(gv);
        reloaded.load(filepath + ".saved");
        BOOST_CHECK(reloaded.compareTo(file));
    }
    std::cout << "Saving " << (filepath + ".raw_genie") << std::endl;
    file.saveRaw(filepath + ".raw_genie");

    // The original dat files have some random uninitialized garbage in them
    // (three bytes in the Graphics::FileName string after the terminating
    // null, mostly 78 01 df, but also things like f8 04 df),
    // so just skip byte-for-byte diffing.
    if (gv == genie::GV_SWGB || gv == genie::GV_CC) {
        return 0;
    }

    return binaryCompare((filepath + ".raw_orig").c_str(),
            (filepath + ".raw_genie").c_str(), 10);
}

BOOST_AUTO_TEST_CASE(simple_read_write_test)
{
    BOOST_TEST_MESSAGE("Simple read write test...");

    BOOST_TEST_MESSAGE("AoK: ");
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_AoE), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_RoR), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_AoK), 0);
    BOOST_REQUIRE_EQUAL(readWriteDiff(genie::GV_TC), 0);

    // optional because I can't be arsed to get them
    BOOST_WARN_EQUAL(readWriteDiff(genie::GV_SWGB), 0);
    BOOST_WARN_EQUAL(readWriteDiff(genie::GV_CC), 0);
}
