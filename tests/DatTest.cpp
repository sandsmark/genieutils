/*
    genieutils - <description>
    Copyright (C) 2011  Armin Preiml <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define BOOST_TEST_MODULE dat_test
#include <boost/test/unit_test.hpp>

#include <string>

#include "genie/dat/DatFile.h"
#include "../src/tools/bincompare/bincomp.h"

const char *const DAT_PATH = "dat/";

const char *getFileName(genie::GameVersion gv)
{
    std::string file_name(DAT_PATH);

    switch (gv) {
    case genie::GV_AoE:
        file_name += "empires.dat";
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
    }

    return file_name.c_str();
}

genie::DatFile *openFile(genie::GameVersion gv)
{
    genie::DatFile *df = new genie::DatFile();
    df->setGameVersion(gv);
    df->load(getFileName(gv));

    return df;
}

int readWriteDiff(genie::GameVersion gv)
{
    std::string fn = std::string(getFileName(gv));
    std::string fn_or = fn + ".raw_orig";
    std::string fn_gc = fn + ".genie";
    std::string fn_gr = fn + ".raw_genie";

    genie::DatFile file;
    file.setGameVersion(gv);

    file.extractRaw(fn.c_str(), fn_or.c_str());

    file.load(fn.c_str());
    file.saveAs(fn_gc.c_str());

    file.extractRaw(fn_gc.c_str(), fn_gr.c_str());

    return binaryCompare(fn_or.c_str(), fn_gr.c_str());
}

BOOST_AUTO_TEST_CASE(simple_read_write_test)
{
    BOOST_TEST_MESSAGE("Simple read write test...");

    BOOST_TEST_MESSAGE("AoK: ");
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_AoE), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_RoR), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_AoK), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_TC), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_SWGB), 0);
    BOOST_CHECK_EQUAL(readWriteDiff(genie::GV_CC), 0);
}
