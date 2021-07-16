/*
    Log handling
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2018 Martin Sandsmark

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

#include "genie/util/Logger.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

namespace genie {

Logger::LogLevel Logger::LOG_LEVEL = L_OFF;

std::ostream *Logger::global_out_ = &std::cout;

//------------------------------------------------------------------------------
Logger &Logger::getRootLogger()
{
    static Logger l;
    return l;
}

//------------------------------------------------------------------------------
Logger &Logger::getLogger(const std::string &name)
{
    static std::unordered_map<std::string, Logger> loggers;
    loggers[name].name_ = name;
    return loggers[name];
}

//------------------------------------------------------------------------------
void Logger::setLogLevel(Logger::LogLevel loglevel)
{
    Logger::LOG_LEVEL = loglevel;
}

void Logger::setGlobalOutputStream(std::ostream &ostream)
{
    global_out_ = &ostream;
}

void Logger::info(const std::string &msg)
{
    log(L_INFO, msg);
}

void Logger::debug(const std::string &msg)
{
    log(L_DEBUG, msg);
}

void Logger::warn(const std::string &msg)
{
    log(L_WARNING, msg);
}

void Logger::error(const std::string &msg)
{
    log(L_ERROR, msg);
}

void Logger::fatal(const std::string &msg)
{
    log(L_FATAL, msg);
}

std::ostream *Logger::getGlobalOutputStream()
{
    return global_out_;
}

void Logger::log(LogLevel loglevel, const std::string &format)
{
    switch(loglevel) {
    case L_INFO:
    case L_DEBUG:
        *global_out_ << "\033[02;32m";
        break;
    case L_WARNING:
        *global_out_ <<  "\033[01;33m";
        break;
    case L_ERROR:
    case L_FATAL:
        *global_out_ <<  "\033[01;31m";
        break;
    case L_OFF:
    default:
        break;
    }
    *global_out_ << getLogLevelName(loglevel) << ": " << format << "\033[0m" << std::endl;
}

//------------------------------------------------------------------------------
std::string Logger::getLogLevelName(Logger::LogLevel loglevel)
{
    static const std::string LOG_LEVEL_NAMES[] = {
        "Info   ", //LINFO
        "Debug  ",
        "Warning",
        "ERROR  ",
        "FATAL  ",
        "Off    " //LOFF
    };

    return LOG_LEVEL_NAMES[loglevel];
}
} // namespace genie
