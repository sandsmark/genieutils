/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2013  Armin Preiml <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "genie/dat/DatFile.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

/// Usage: gextract file.dat extracted_file.dat
int main(int argc, char **argv)
{
  try
  {
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help,h", "show help")
//       ("raw-in", "input file is uncompressed")
      ("raw-out", "output file will not be compressed")
      ("geniedat", "load file with geniedat")
      ("game,g", po::value<std::string>(), 
                         "allowed values: aoe, ror, aok, tc, swgb or cc")
      ("input-file,f",  po::value<std::string>(), "input file")
      ("output-file,o", po::value<std::string>(), "output file")
      ("verbose,v", "verbose output")
    ;
    
    po::positional_options_description pos;
    pos.add("input-file", 1);
    pos.add("output-file",1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos).run() , vm);
    po::notify(vm);
        
    if (vm.count("help") || 
        !(vm.count("input-file") && vm.count("output-file")))
    {
      std::cout << "Usage: " << argv[0] 
              << " [OPTION]... --geniedat -g GAMETYPE INPUT-FILE OUTPUT-FILE\n" 
//               << " [OPTION]... --raw-in INPUT-FILE OUTPUT-FILE\n" 
              << " [OPTION]... --raw-out INPUT-FILE OUTPUT-FILE\n" 
              << std::endl;
      
      std::cout << "Default operation is reading a compressed dat file and extract it\n"
                << std::endl;
      std::cout << desc << std::endl;
      return 0;
    }
    
    genie::DatFile file;
    
    if (vm.count("verbose"))
      file.setVerboseMode(true);
    
    if (vm.count("raw-out"))
    {
      file.extractRaw(vm["input-file"].as< std::string >().c_str(), 
                      vm["output-file"].as< std::string >().c_str());
    }
    else if (vm.count("geniedat"))
    {
      if (vm["game"].as< std::string >() == "aoe")
        file.setGameVersion(genie::GV_AoE);
      if (vm["game"].as< std::string >() == "ror")
        file.setGameVersion(genie::GV_RoR);
      if (vm["game"].as< std::string >() ==  "aok")
        file.setGameVersion(genie::GV_AoK);
      if (vm["game"].as< std::string >() == "tc")
        file.setGameVersion(genie::GV_TC);
      if (vm["game"].as< std::string >() == "swgb")
        file.setGameVersion(genie::GV_SWGB);
      if (vm["game"].as< std::string >() == "cc")
        file.setGameVersion(genie::GV_CC);
      
      if (file.getGameVersion() == genie::GV_None)
      {
        std::cout << "Wrong game arg\n" << std::endl;
        
        std::cout << desc << std::endl;
        return 0;
      }

      file.load(vm["input-file"].as< std::string >().c_str());
      
      file.saveAs(vm["output-file"].as< std::string >().c_str()); 
    }    
    
  }
  catch (boost::exception_detail::clone_impl<
          boost::exception_detail::error_info_injector<
            boost::program_options::too_many_positional_options_error> > e)
  {
    std::cout << "too many arguments!" << std::endl;
  }
  
             
  return 0;
}
