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
#include <fstream>
#include <iostream>

//

int binaryCompare(const char * file1, const char * file2)
{
 
  std::ifstream f1(file1, std::ifstream::binary), f2(file2, std::ifstream::binary);
  
  int cnt = 0;
    
  while (!f1.eof() && !f2.eof())
  {
    char c1, c2;
    
    f1.read(&c1,1);
    f2.read(&c2,1);
    
    
    if (c1 != c2)
    {
      std::cout << f1.tellg() << ": " << std::hex << (short)c1 << " " 
                << (short)c2 << std::dec << std::endl;
      cnt ++;
    }
    
  }
  
  std::cout << cnt << " diffs!" << std::endl;
  
  if (f1.eof() != f2.eof())
  {
    std::cout << "EOF diff!!" << std::endl;
    return -1;
  }
  else
    return cnt;
}