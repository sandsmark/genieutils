/*
    Various things to make life better, mostly because STL is crap (I blame boost)
    Copyright (C) 2019  Martin Sandsmark <martin.sandsmark@kde.org>

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

#include "genie/util/Utility.h"

#include <errno.h>
#include <stdlib.h>
#include <filesystem>
#include <iostream>

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#else
extern "C" {
#include <unistd.h>
}
#endif

namespace genie {

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
static std::string wintendoExePath()
{
    std::vector<CHAR> pathBuf;
    DWORD ret;
    do {
        pathBuf.resize(pathBuf.size() + MAX_PATH);
        ret = GetModuleFileName(NULL, pathBuf.data(), DWORD(pathBuf.size()));

        // Windows APIs are a special kind of retarded, 0 means it failed
        if (ret == 0) {
            std::cerr << "Failed to query wintendo exe path" << std::endl;
            return {};
        }
    } while(ret >= pathBuf.size());

    return std::string(pathBuf.data(), ret);
}
#else
static std::string procExePath()
{
    const std::string path = "/proc/" + std::to_string(getpid()) + "/exe";
    if (!std::filesystem::is_symlink(path)) {
        std::cerr << path << " is not a valid symlink" << std::endl;
        return {};
    }

    return std::filesystem::read_symlink(path);
}
#endif



std::string util::executablePath()
{
    std::string path;
#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
    path = wintendoExePath();
#else
    path = procExePath();
#endif
    if (path.empty()) {
        std::cerr << "Failed to resolve executable path" << std::endl;
        path = std::filesystem::current_path().string();
    }

    return path;
}

std::string genie::util::resolvePathCaseInsensitive(const std::string &inputPath, const std::string &basePath)
{
    if (!std::filesystem::exists(basePath)) {
        return {};
    }

    if (std::filesystem::exists(basePath + '/' + inputPath)) {
        return basePath + '/' + inputPath;
    }

    const std::vector<std::string> pathParts = util::stringSplit(inputPath, '/');
    std::string compareFilename = util::toLowercase(pathParts[0]);

    std::string correct;
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(basePath)) {
        std::string candidate = entry.path().filename().string();
        if (util::toLowercase(candidate) == compareFilename) {
            correct = candidate;
            break;
        }
    }

    if (correct.empty()) {
        std::cerr << "Failed to resolve " << basePath << " " << inputPath << std::endl;
        return {};
    }

    if (pathParts.size() > 1) {
        std::string newInput; // can't be arsed to clean up duplicate /'s and whatnot beforehand
        for (size_t i=1; i<pathParts.size(); i++) {
            newInput += "/" + pathParts[i];
        }
        return resolvePathCaseInsensitive(newInput, basePath + '/' + correct);
    }

    return basePath + '/' + correct;
}

std::string util::executableDirectory()
{
   return std::filesystem::path(executablePath()).remove_filename().string();
}

} // namespace genie
