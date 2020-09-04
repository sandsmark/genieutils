/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P

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

#include "genie/resource/BlendomaticFile.h"

#include <stdexcept>
#include <chrono>
#include <cmath>

#include "genie/resource/BlendomaticFile.h"

namespace genie {

Logger &BlendomaticFile::log = Logger::getLogger("genie.BlendomaticFile");

const BlendMode BlendMode::null;

//------------------------------------------------------------------------------
BlendomaticFile::BlendomaticFile() :
    IFile()
{
}

//------------------------------------------------------------------------------
void BlendomaticFile::serializeObject()
{
    // TODO: SWGB stores this in the dat file, so I need to buy SWGB and figure out what overrides what

    serializeSize<uint32_t>(modeCount_, modes_.size());
    serialize<uint32_t>(tileCount_);

    if (isOperation(OP_READ)) {
        if (overrideModeCount > 0) {
            modeCount_ = overrideModeCount;
        }
    }

    modes_.resize(modeCount_);

    for (uint32_t i = 0; i < modeCount_; i++) {
        log.debug("reading mode %", i);

        // number of pixels
        serialize<uint32_t>(modes_[i].pixelCount);

        if (modes_[i].pixelCount > 3000) {
            std::cerr << "invalid pixel count " << modes_[i].pixelCount << std::endl;
            exit(0);
        }

        // TODO:
        // we should check these and skip the alphaValues reading in case any tiles don't have alpha
        serialize<uint8_t>(modes_[i].tileHasAlpha, tileCount_);

        serialize<uint32_t>(modes_[i].alphaBitmap, modes_[i].pixelCount);

        // alpha values from 0-128
        serialize<uint8_t>(modes_[i].alphaValues, tileCount_, modes_[i].pixelCount);
    }
}

//------------------------------------------------------------------------------
void BlendomaticFile::unload()
{
    modes_.clear();
    modeCount_ = 0;
    tileCount_ = 0;
}

void BlendomaticFile::setBlendMode(uint32_t number, const BlendMode &mode)
{
    if (number >= modes_.size()) {
        modes_.resize(number + 1);
    }

    modes_[number] = mode;
}

} // namespace genie
