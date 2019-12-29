/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2015 - 2017  Mikko "Tapsa" P

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

#ifndef GENIE_TERRAINCOMMON_H
#define GENIE_TERRAINCOMMON_H
#include "genie/file/ISerializable.h"

namespace genie {

class FrameData : public ISerializable
{
public:
    int16_t FrameCount = 0;
    int16_t AngleCount = 0;
    int16_t ShapeID = 0;


    bool compareTo(const FrameData &other) const;

private:
    void serializeObject(void) override;
};

class SharedTerrain : public ISerializable
{
public:
    /// Was 17 before increased to 19.
    static constexpr unsigned short TILE_TYPE_COUNT = 19;

    /// Must be one or the game will crash
    int8_t Enabled = 0;

    int8_t Random = 0;

    virtual size_t getNameSize(void) = 0;

    /// Internal long name
    std::string Name = "";

    /// Internal SLP name
    std::string Name2 = "";

    /// SLP obviously
    int32_t SLP = -1;

    /// Pointer to loaded SLP (when object instatiated). You probably don't want to use this
    int32_t ShapePtr = 0;

    /// The sound to play when visible
    int32_t SoundID = -1;

    /// Minimap colors
    std::array<uint8_t, 3> Colors = { 0, 0, 0 };

    /// If the terrain has animated graphics
    int8_t IsAnimated = 0;

    /// Number of frames to animate through
    int16_t AnimationFrames = 0;

    /// Number of frames to pause animation after last frame is drawn
    int16_t PauseFames = 0;

    /// Time between frames
    float Interval = 0;

    /// Time to pause after last frame
    float PauseBetweenLoops = 0;

    /// The current frame (includes animation & pause frames)
    int16_t Frame = 0;

    /// The current frame to draw
    int16_t DrawFrame = 0;

    /// Last time animation frame was changed
    float AnimateLast = 0;

    /// Has the DrawFrame changed since terrain was drawn?
    int8_t FrameChanged = 0;

    /// Has the DrawFrame been drawn already?
    int8_t Drawn = 0;
};
} // namespace genie

#endif // GENIE_TERRAINCOMMON_H
