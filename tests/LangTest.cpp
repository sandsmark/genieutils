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

    BOOST_CHECK_EQUAL(lf.getString(42320).compare("Total food collected by each player."), 0);
    BOOST_CHECK_EQUAL(lf.getString(4442).compare("King Wallia"), 0);

    lf.setString(42320, "Test pcrio ftw! U: ö ü ä ÜÄÖ haha");

    lf.saveAs("new_lang.dll");
    lf.load("new_lang.dll");

    std::cout << "GetSaved String: \"" << lf.getString(42320) << "\"" << std::endl;

    lf.setString(1, "new one");
    std::cout << lf.getString(1) << std::endl;

    std::cout << "Loading aoe lang: " << std::endl;
    lf.load("lang/aoe/language.dll");
    lf.getString(54518);

    genie::LangFile *lptr = new genie::LangFile();

    lptr->load("lang/sw/language.dll");
    lptr->getString(3064);

    delete lptr;
}

BOOST_AUTO_TEST_CASE(random_write)
{
    genie::Logger::setLogLevel(genie::Logger::L_INFO);
    std::string randStr;
    genie::LangFile lf;

    std::string langFilename = LANG_PATH;
    langFilename += "aok/language.dll";
    lf.load(langFilename.c_str());

    srand(time(NULL));

    int i, j;

    for (j = 0; j < 10000; j++) {
        int wordLen = rand() % 30;

        for (i = 0; i < wordLen; i++) {
            char c = rand() % 26 + 65;

            randStr += c;
        }

        lf.setString(rand() % 10000, randStr);

        randStr = "";
    }

    lf.saveAs("temp.dat");
}

/// Testing language file containting one language, but 2 different codepages
BOOST_AUTO_TEST_CASE(diff_codepage_test)
{

    genie::LangFile lf;

    std::string lfName = LANG_PATH;
    lfName += "diff_cp.dll";

    lf.load(lfName.c_str());

    BOOST_CHECK_EQUAL(lf.getString(1).compare("Diff codepage"), 0);
}

BOOST_AUTO_TEST_CASE(fail_read_test)
{
    genie::LangFile lf;

    BOOST_CHECK_THROW(lf.load("someunexisting..asdf"), genie::PcrioError);
}
