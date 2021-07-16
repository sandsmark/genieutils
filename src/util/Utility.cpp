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
#include "genie/util/Logger.h"

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
static Logger log = Logger::getLogger("genie.Utility");

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
static std::string wintendoExePath()
{
    std::vector<CHAR> pathBuf;
    DWORD ret;
    do {
        pathBuf.resize(pathBuf.size() + MAX_PATH);
        ret = GetModuleFileNameA(NULL, pathBuf.data(), DWORD(pathBuf.size()));

        // Windows APIs are a special kind of retarded, 0 means it failed
        if (ret == 0) {
            log.error("Failed to query wintendo exe path");
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
        log.error("% is not a valid symlink", path);
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
        log.error("Failed to resolve executable path");
        path = std::filesystem::current_path().string();
    }

    return path;
}

// Don't try to replace this with std::filesystem::canonical-blah
// std::filesystem is from boost and therefore is a shit show and doesn't do what you expect.
inline std::string cleanPath(const std::string &input)
{
    if (input.empty()) {
        return input;
    }

    std::string ret;
    ret.push_back(input[0]);
    for (size_t i=1; i<input.size(); i++) {
        if (input[i-1] == '/' && input[i] == '/') {
            continue;
        }

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
        if (input[i-1] == '\\' && input[i] == '\\') {
            continue;
        }
#endif
        ret.push_back(input[i]);
    }

    return ret;
}

#if defined(__linux__)
#include <wordexp.h>

// Resolves ~ etc.
static std::string resolvePath(const char *path)
{
    if (!path) {
        return {};
    }

    wordexp_t expanded;
    wordexp(path, &expanded, 0);
    std::string resolved(expanded.we_wordv[0]);
    wordfree(&expanded);

    return resolved;
}
#endif

std::string genie::util::resolvePathCaseInsensitive(std::string inputPath, const std::string &basePath)
{
#if defined(__linux__)
    if (inputPath.find("~") != std::string::npos) {
        puts("resolving wordexp");
        inputPath = resolvePath(inputPath.c_str());
    }
#endif
    if (inputPath.find("..") != std::string::npos) {
        inputPath = std::filesystem::absolute(std::filesystem::path(inputPath)).string();
    }

    if (!std::filesystem::exists(basePath)) {
        return {};
    }

    if (std::filesystem::exists(basePath + '/' + inputPath)) {
        return cleanPath(basePath + '/' + inputPath);
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
        log.debug("Failed to resolve % in folder %", inputPath, basePath);
        return {};
    }

    if (pathParts.size() > 1) {
        std::string newInput; // can't be arsed to clean up duplicate /'s and whatnot beforehand
        for (size_t i=1; i<pathParts.size(); i++) {
            newInput += "/" + pathParts[i];
        }
        return cleanPath(resolvePathCaseInsensitive(newInput, basePath + '/' + correct));
    }

    return cleanPath(basePath + '/' + correct);
}

std::string util::executableDirectory()
{
   return std::filesystem::path(executablePath()).remove_filename().string();
}

} // namespace genie
