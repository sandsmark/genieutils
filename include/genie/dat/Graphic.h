/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P

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

#pragma once
#include "genie/file/ISerializable.h"
#include "GraphicDelta.h"
#include "GraphicAttackSound.h"

namespace genie {

class Graphic : public ISerializable
{
public:
    void setGameVersion(GameVersion gv) override;

    /// Returns the size of Name
    unsigned short getNameSize();

    /// Returns the size of FileName
    unsigned short getFilenameSize();

    /// These two name strings appear to be equal in all graphics, so the
    /// second one's use is unknown. Like the genie unit's internal name value,
    /// they are heavily abbreviated, such as "ARRG2NNE" meaning "Archery
    /// Range Fuedal Age (orth) European"
    std::string Name = "";
    std::string FileName = "";
    std::string ParticleEffectName = ""; // TODO: doc, new in DE

    /// Number of frames to skip
    uint16_t FirstFrame = 0;

    /// SLP resource id
    union {
        /// SLP resource id
        int32_t SLP = -1;
        int16_t ResourceIdS16;
    };

    //TODO
    int8_t IsLoaded = 0;
    int8_t OldColorFlag = 0;

    /// The layer describes order of graphics being rendered.
    /// Possible values: 0 (lowest layer) to 40 (highest layer)
    /// Graphics on a higher layer will be rendered above graphics of a lower
    /// layer. If graphics share the same layer, graphics will be displayed
    /// dependend on their map positions.
    ///
    /// Draw Level
    /// 0   Terrain
    /// 5   Shadows, farms
    /// 6   Rubble
    /// 10   Constructions, corpses, shadows, flowers, ruins
    /// 11   Fish
    /// 19   Rugs, craters
    /// 20   Buildings, units, damage flames, mill animation
    /// 21   Blacksmith smoke
    /// 22   Hawk
    /// 30   Projectiles, explosions
    int8_t Layer = 0;

    /// Setting this to 0 forces the player color to be blue.
    int8_t PlayerColor = -1;

    /// Makes the graphic change its player color according to nearby units
    int8_t Rainbow = -1;

    /// 0   Not selectable
    /// 1   Select only on pixels
    /// 2   Select in box
    enum SelectionType {
        NotSelectable = 0,
        SelectOnPixels = 1,
        SelectInBox = 2
    };

    int8_t TransparentSelection = 0;

    /// Bounding Box Coordinates (X1, Y1, X2, Y2)
    std::array<int16_t, 4> Coordinates = { 0, 0, 0, 0 };

    int16_t SoundID = -1;
    uint32_t WwiseSoundID = 0; // TODO doc: new in DE
    int8_t AngleSoundsUsed = 0;

    /// Number of frames per angle animation
    uint16_t FrameCount = 0;

    /// Number of angles tored in slp and also the number of extra structures.
    /// If there are more than 1 angle, AngleCount/2 - 1 frames will be
    /// mirrored. That means angles starting from south going clockwise to
    /// north are stored and the others will be mirrored.
    uint16_t AngleCount = 0;

    /// If this is over 0, the speed of the unit will be replaced with this.
    float SpeedMultiplier = 0;

    /// Frame rate in seconds. (Delay between frames)
    float FrameDuration = 0;

    float AnimationDuration = 0; // TODO doc, new in DE

    /// Time to wait until the animation sequence is started again.
    float ReplayDelay = 0;

    /** Bit field
     */
    enum SequenceTypeFlags {
        SequenceAnimated = 0x1,
        SequenceDirectional = 0x2,
        SequenceRandomized = 0x4,
        SequenceOnce = 0x8
    };
    uint8_t SequenceType = 0;
    int16_t ID = -1;
    int8_t MirroringMode = 0;
    int8_t EditorFlag = 0;

    std::vector<GraphicDelta> Deltas;
    std::vector<GraphicAngleSound> AngleSounds;

    bool compareTo(const Graphic &other) const;

private:
    void serializeObject() override;
};
} // namespace genie

