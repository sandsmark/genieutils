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
    file->setGameVersion(gv);
    file->load();
    return file;
}

void writeReadCheck(const genie::GameVersion gv)
{
    genie::Logger::setLogLevel(genie::Logger::L_INFO);
    std::cout << "Testing game version " << genie::DatFile::versionName(gv) << std::endl;

    return; // TODO: fixme

    genie::DatFile writeFile;
    writeFile.setVerboseMode(true);
    writeFile.setGameVersion(gv);

    // So far we need to set this (and it still fails), shouldn't need to
//    writeFile.FileVersion = "VER 0.0";
//    writeFile.TerrainsUsed1 = 1;
//    writeFile.TerrainRestrictions.resize(1);
//    writeFile.TerrainRestrictions[0].PassableBuildableDmgMultiplier.resize(1);
//    writeFile.TerrainRestrictions[0].TerrainPassGraphics.resize(1);
//    writeFile.TerrainPassGraphicPointers.resize(1);
//    writeFile.FloatPtrTerrainTables.resize(1);
//    writeFile.Graphics.resize(42);
//    writeFile.GraphicPointers.resize(42);
//    writeFile.PlayerColours.resize(42);
//    writeFile.Sounds.resize(1337);
//    writeFile.TerrainBlock.Terrains.resize(genie::Terrain::getTerrainCount(gv));
//    writeFile.Graphics.resize(42);

//    writeFile.RandomMaps.Maps.resize(10);
//    writeFile.RandomMaps.Maps[0].MapLands.resize(1);
//    writeFile.RandomMaps.Maps[0].MapUnits.resize(1);
//    writeFile.RandomMaps.Maps[0].MapTerrains.resize(1);
//    writeFile.RandomMaps.Maps[0].MapElevations.resize(1);

//    writeFile.Effects.resize(42);
//    writeFile.UnitLines.resize(42);
//    writeFile.UnitHeaders.resize(42);
//    writeFile.Civs.resize(1);
//    writeFile.Civs[0].Units.resize(6);
//    writeFile.Civs[0].UnitPointers.resize(6);

    writeFile.Civs[0].Units[4].Moving.TrackingUnit = 5;
    writeFile.Civs[0].Units[4].Creatable.TrainTime = 999;
    writeFile.Civs[0].Units[4].Creatable.ButtonID = 100;

    writeFile.Civs[0].Units[5].Moving.TrackingUnit = 5;
    writeFile.Civs[0].Units[5].Creatable.TrainTime = 999;
    writeFile.Civs[0].Units[5].Creatable.ButtonID = 100;

    genie::Unit a = writeFile.Civs[0].Units[5];
    genie::Unit b(a);
    writeFile.saveAs("temp.dat");
    std::cout << "Size after saving " << writeFile.Civs.size() << std::endl;


    genie::DatFile readFile;
    readFile.setVerboseMode(true);
    readFile.setGameVersion(gv);
    readFile.load("temp.dat");
    BOOST_CHECK(readFile.compareTo(writeFile));

    BOOST_CHECK_GT(readFile.Civs.size(), 0);
    if (readFile.Civs.empty()) {
        return;
    }

    BOOST_CHECK( readFile.Civs[0].Units[4].Moving.TrackingUnit == 5 );
    BOOST_CHECK( readFile.Civs[0].Units[4].Creatable.TrainTime == 999 );
    BOOST_CHECK( readFile.Civs[0].Units[4].Creatable.ButtonID == 100 );

    BOOST_CHECK( readFile.Civs[0].Units[5].Moving.TrackingUnit == 5 );
    BOOST_CHECK( readFile.Civs[0].Units[5].Creatable.TrainTime == 999 );
    BOOST_CHECK( readFile.Civs[0].Units[5].Creatable.ButtonID == 100 );

    BOOST_CHECK( a.Moving.TrackingUnit == 5 );
    BOOST_CHECK( a.Creatable.TrainTime == 999 );
    BOOST_CHECK( a.Creatable.ButtonID == 100 );

    BOOST_CHECK( b.Moving.TrackingUnit == 5 );
    BOOST_CHECK( b.Creatable.TrainTime == 999 );
    BOOST_CHECK( b.Creatable.ButtonID == 100 );
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
    std::unique_ptr<genie::DatFile> file = openFile(genie::GV_TC);
    BOOST_REQUIRE(file);

    std::cout << file->Civs.size() << std::endl;
    BOOST_REQUIRE_GT(file->Civs.size(), 0);

    file->Civs[0].Units[4].Moving.TrackingUnit = 5;
    file->Civs[0].Units[4].Creatable.TrainTime = 999;
    file->Civs[0].Units[4].Creatable.ButtonID = 100;

    file->Civs[0].Units[5].Moving.TrackingUnit = 5;
    file->Civs[0].Units[5].Creatable.TrainTime = 999;
    file->Civs[0].Units[5].Creatable.ButtonID = 100;

    genie::Unit a = file->Civs[0].Units[5];
    file->saveAs("temp.dat");


    genie::DatFile readFile;
    readFile.setGameVersion(genie::GV_TC);
    readFile.load("temp.dat");

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
    std::cout << "Testing version " << genie::DatFile::versionName(gv) << std::endl;
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

    return binaryCompare((filepath + ".raw_orig").c_str(),
                         (filepath + ".raw_genie").c_str());
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
