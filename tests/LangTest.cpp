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

#define BOOST_TEST_MODULE lang_test
#include <boost/test/unit_test.hpp>

#include <string>
#include <genie/lang/LangFile.h>

#include <time.h>

const char *const LANG_PATH = "lang/";

BOOST_AUTO_TEST_CASE(simple_read_write_test)
{
    genie::Logger::setLogLevel(genie::Logger::L_INFO);

    genie::LangFile lf;

    std::string langFilename = LANG_PATH;
    langFilename += "aok/language.dll";
    lf.load(langFilename.c_str());

    BOOST_CHECK_EQUAL(lf.getString(42320), "Total food collected by each player.");
    BOOST_CHECK_EQUAL(lf.getString(4442), "King Wallia");

    lf.setString(42320, "Test pcrio ftw! U: ö ü ä ÜÄÖ haha");

    lf.saveAs("new_lang.dll");
    lf.load("new_lang.dll");

    BOOST_CHECK_EQUAL(lf.getString(42320), "Test pcrio ftw! U: ö ü ä ÜÄÖ haha");

    lf.setString(1, "new one");
    BOOST_CHECK_EQUAL(lf.getString(1), "new one");

    lf.load("lang/aoe/language.dll");
    BOOST_CHECK_EQUAL(lf.getString(54518), "Click to order units to repair a building or boat.");

    genie::LangFile *lptr = new genie::LangFile();

    lptr->load("lang/sw/language.dll");
    BOOST_CHECK_EQUAL(lptr->getString(3064), "Click where you want the unit(s) to be ejected.");

    delete lptr;
}

BOOST_AUTO_TEST_CASE(random_write)
{
    genie::Logger::setLogLevel(genie::Logger::L_INFO);
    genie::LangFile saveFile;

    std::string langFilename = LANG_PATH;
    langFilename += "aok/language.dll";
    saveFile.load(langFilename.c_str());

    srand(1); // consistent random

    int i, j;

    std::unordered_map<int, std::string> written;
    saveFile.setDefaultCharset("WINDOWS-1251");
    for (j = 0; j < 10000; j++) {
        std::string randStr;
        int wordLen = rand() % 30;

        for (i = 0; i < wordLen; i++) {
            char c = (rand() % 254) + 1; // TODO: it depends on strlen and doesn't handle strings with nulls. I think it maybe should

            randStr += c;
        }

        const int id = rand() % 10000;

        saveFile.setString(id, randStr);
        written[id] = saveFile.convertTo(randStr, saveFile.getDefaultCodepage());
    }

    saveFile.saveAs("temp.dat");

    genie::LangFile loadFile;
    loadFile.load("temp.dat");

    for (const std::pair<const int, std::string> &p : written) {
        BOOST_CHECK_EQUAL(loadFile.getString(p.first), p.second);
    }
}

/// Testing language file containting one language, but 2 different codepages
BOOST_AUTO_TEST_CASE(diff_codepage_test)
{
    // TODO: need to find out where diff_cp.dll came from
    return;

    genie::LangFile lf;

    std::string lfName = LANG_PATH;
    lfName += "diff_cp.dll";

    lf.load(lfName.c_str());

    BOOST_CHECK_EQUAL(lf.getString(1), "Diff codepage");
}

BOOST_AUTO_TEST_CASE(fail_read_test)
{
    genie::LangFile lf;

    BOOST_CHECK_THROW(lf.load("someunexisting..asdf"), genie::PcrioError);
}
