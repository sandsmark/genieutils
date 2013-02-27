#define BOOST_TEST_MODULE geniedat_torture_chamber

#include <iostream>


#include <boost/test/unit_test.hpp>
#include "genie/dat/File.h"
#include "../bincompare/bincomp.h"


const char * const DAT_PATH = "tmp/";

std::string getFileName(genie::GameVersion gv)
{
  std::string file_name(DAT_PATH);
  
  switch(gv)
  {
    case genie::GV_AoE: file_name += "Empires.dat"; break;
    case genie::GV_RoR: file_name += "empires_x1.dat"; break;
    case genie::GV_AoK: file_name += "empires2.dat"; break;
    case genie::GV_TC: file_name += "empires2_x1_p1.dat"; break;
    case genie::GV_SWGB: file_name += "genie.dat"; break;
    case genie::GV_CC: file_name += "genie_x1.dat"; break;
    
    case genie::GV_None: break;
  }
  
  return file_name;
}

genie::File *openFile(genie::GameVersion gv)
{
  return new genie::File(getFileName(gv), gv);
}

BOOST_AUTO_TEST_CASE( simple_change_values_test )
{
  genie::File *file = openFile(genie::GV_TC);
  
  std::cout << file->Civs[0].size() << std::endl;
  
  /*file->Civs[0].Units[4].DeadFish.TrackingUnit = 5;
  file->Civs[0].Units[4].Creatable.TrainTime = 999;
  file->Civs[0].Units[4].Creatable.ButtonID = 100;
  
  file->Civs[0].Units[5].DeadFish.TrackingUnit = 5;
  file->Civs[0].Units[5].Creatable.TrainTime = 999;
  file->Civs[0].Units[5].Creatable.ButtonID = 100;
  
  genie::Unit a = file->Civs[0].Units[5];
  genie::Unit b(a);
  
  file->save("temp.dat");
  
  file->load("temp.dat");
  
  BOOST_CHECK( file->Civs[0].Units[4].DeadFish.TrackingUnit == 5 );
  BOOST_CHECK( file->Civs[0].Units[4].Creatable.TrainTime == 999 );
  BOOST_CHECK( file->Civs[0].Units[4].Creatable.ButtonID == 100 );
  
  BOOST_CHECK( file->Civs[0].Units[5].DeadFish.TrackingUnit == 5 );
  BOOST_CHECK( file->Civs[0].Units[5].Creatable.TrainTime == 999 );
  BOOST_CHECK( file->Civs[0].Units[5].Creatable.ButtonID == 100 );
  
  BOOST_CHECK( a.DeadFish.TrackingUnit == 5 );
  BOOST_CHECK( a.Creatable.TrainTime == 999 );
  BOOST_CHECK( a.Creatable.ButtonID == 100 );
  
  BOOST_CHECK( b.DeadFish.TrackingUnit == 5 );
  BOOST_CHECK( b.Creatable.TrainTime == 999 );
  BOOST_CHECK( b.Creatable.ButtonID == 100 ); */
}

int readWriteDiff(genie::GameVersion gv)
{
  genie::File file;
  file.setGameVersion(gv);
  
  file.extractRaw(getFileName(gv), getFileName(gv) + ".raw_orig");
  file.load(getFileName(gv));
  file.save(getFileName(gv) + ".raw_genie", true);
  
  return binaryCompare((getFileName(gv) + ".raw_orig").c_str(), 
                       (getFileName(gv) + ".raw_genie").c_str());
}

BOOST_AUTO_TEST_CASE( simple_read_write_test )
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
