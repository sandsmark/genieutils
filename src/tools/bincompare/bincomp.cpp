/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml

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

int binaryCompare(const char *file1, const char *file2, int max)
{

    std::ifstream f1(file1, std::ifstream::binary), f2(file2, std::ifstream::binary);

    int cnt = 0;

    while (!f1.eof() && !f2.eof()) {
        uint8_t c1, c2;

        f1.read((char*)&c1, 1);
        f2.read((char*)&c2, 1);

        if (c1 != c2) {
            if (max == -1 || cnt < max) {
                std::cout << f1.tellg() << ": " << std::hex << short(c1) << " "
                    << short(c2) << std::dec << std::endl;
            }
            cnt++;
        }
    }
    if (max != -1 && cnt >= max) {
        std::cout << "Too many changes, diff truncated to " << max << std::endl;
    }

    std::cout << cnt << " diffs!" << std::endl;
    if (cnt > 0) {
        std::cout << "File A: " << file1 << std::endl;
        std::cout << "File B: " << file2 << std::endl;

    }

    if (f1.eof() != f2.eof()) {
        std::cout << "EOF diff!!" << std::endl;
        return -1;
    } else {
        return cnt;
    }
}
