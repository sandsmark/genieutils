#include <iostream>
#include <vector>
#include <genie/resource/PalFile.h>
#include <fstream>
#include <genie/resource/SlpFile.h>
#include <genie/resource/SlpFrame.h>
#include "genie/resource/DrsFile.h"
#include "genie/resource/SlpFile.h"
#include "genie/resource/Color.h"
#include <genie/script/ScnFile.h>
#include <genie/file/Compressor.h>
#include <genie/lang/LangFile.h>
#include <boost/iostreams/copy.hpp>

void testLang()
{
  genie::LangFile lf;

  lf.load("language.dll");

  std::cout << "GetString: " << lf.getString(42320) << std::endl;

  lf.setString(42320, "Test pcrio ftw! U: ö ü ä ÜÄÖ haha");

  std::cout << "GetOtherString: " << lf.getString(4442) << std::endl;

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

int main(int argc, char **argv)
{
  genie::Logger::setLogLevel(genie::Logger::L_INFO);

  std::ofstream log_out;
  log_out.open("log.txt");

  genie::Logger::setGlobalOutputStream(log_out);

  testLang();

  /*
  std::ifstream ifs;
  std::ofstream ofs;

  ifs.open("tmp/empires2.dat", std::ios::binary);
  ofs.open("q.dat", std::ios::binary);

  genie::Compressor c;

  std::shared_ptr<std::istream> unc = c.startDecompression(&ifs);

  boost::iostreams::copy((*unc), ofs);

  c.stopDecompression();

//   delete unc;

  ifs.close();
  ofs.close();
   */

  return 0;
}
