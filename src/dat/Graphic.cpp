/*
    genie/dat - A library for reading and writing data files of genie
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

#include "genie/dat/Graphic.h"
#include "genie/util/Utility.h"

#include "TestHelpers.h"

#include <string.h>
#include <iostream>

namespace genie {

//------------------------------------------------------------------------------
void Graphic::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(Deltas);
    updateGameVersion(AngleSounds);
}

unsigned short Graphic::getNameSize()
{
    if (getGameVersion() < GV_SWGB) {
        return 21;
    } else {
        return 25;
    }
}

unsigned short Graphic::getFilenameSize()
{
    if (getGameVersion() < GV_SWGB) {
        return 13;
    } else {
        return 25;
    }
}

bool Graphic::compareTo(const Graphic &other) const
{
    COMPARE_MEMBER(AnimationDuration);
    COMPARE_MEMBER(FrameDuration);
    COMPARE_MEMBER(ReplayDelay);
    COMPARE_MEMBER(SpeedMultiplier);
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(SoundID);
    COMPARE_MEMBER(SLP);
    COMPARE_MEMBER(AngleSoundsUsed);
    COMPARE_MEMBER(EditorFlag);
    COMPARE_MEMBER(IsLoaded);
    COMPARE_MEMBER(Layer);
    COMPARE_MEMBER(MirroringMode);
    COMPARE_MEMBER(OldColorFlag);
    COMPARE_MEMBER(PlayerColor);
    COMPARE_MEMBER(Rainbow);
    COMPARE_MEMBER(TransparentSelection);
    COMPARE_MEMBER(FileName);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER(ParticleEffectName);
    COMPARE_MEMBER_OBJ_VEC(AngleSounds);
    COMPARE_MEMBER_OBJ_VEC(Deltas);
    COMPARE_MEMBER_VEC(Coordinates);
    COMPARE_MEMBER(AngleCount);
    COMPARE_MEMBER(FirstFrame);
    COMPARE_MEMBER(FrameCount);
    COMPARE_MEMBER(WwiseSoundID);
    COMPARE_MEMBER(SequenceType);

    return true;
}

void Graphic::serializeObject()
{
    GameVersion gv = getGameVersion();

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serialize(Name, getNameSize());
        serialize(FileName, getFilenameSize());
    } else {
        serializeDebugString(Name);
        serializeDebugString(FileName);

        if (gv >= GV_C5 && gv <= GV_LatestDE2) {
            serializeDebugString(ParticleEffectName);
        }
        if (gv >= GV_T3 && gv < GV_C2) {
            serialize<uint16_t>(FirstFrame);
        }
    }

    if (gv < GV_TEST) {
        serialize<int16_t>(ResourceIdS16);
        SLP = ResourceIdS16;
    } else {
        serialize<int32_t>(SLP);
    }

    serialize<int8_t>(IsLoaded); // Unused
    serialize<int8_t>(OldColorFlag); // Unused
    serialize<int8_t>(Layer);
    serialize<int8_t>(PlayerColor);
    serialize<int8_t>(Rainbow); // 2nd half of player color
    serialize<int8_t>(TransparentSelection);

    serialize<int16_t, 4>(Coordinates);

    uint16_t delta_count{};
    serializeSize<uint16_t>(delta_count, Deltas.size());
    serialize<int16_t>(SoundID);

    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseSoundID);
    }

    serialize<int8_t>(AngleSoundsUsed);
    serialize<uint16_t>(FrameCount);
    serialize<uint16_t>(AngleCount);
    serialize<float>(SpeedMultiplier);

    if (isOperation(OP_WRITE)) {
        const float newDuration =  FrameCount ? AnimationDuration / FrameCount : 0;

        // There was something wonky (the LSB in the significand of the floats
        // was always set to 0) when the original files were created, so to
        // avoid any diff when just loading and re-writing the original files
        // we have to do this.
        if (!util::floatsEquals(newDuration, FrameDuration)) {
            FrameDuration = newDuration;
        }
    }
    serialize<float>(FrameDuration);

    AnimationDuration = FrameDuration * FrameCount;
    serialize<float>(ReplayDelay);
    serialize<uint8_t>(SequenceType);
    serialize<int16_t>(ID);
    serialize<int8_t>(MirroringMode);

    if (gv >= GV_AoKB) { // 10.72
        serialize<int8_t>(EditorFlag); // A sprite editor thing
    }

    serialize(Deltas, delta_count);

    if (AngleSoundsUsed != 0) {
        if (isOperation(OP_WRITE) && AngleSounds.size() != AngleCount) {
            std::cerr << "Warning: Size mismatch between angle sounds and angles!"
                      << std::endl;
            //Be naughty and force the size to be correct.
            AngleSounds.resize(AngleCount, AngleSounds.front());
        }

        serialize(AngleSounds, AngleCount);
    }
}
} // namespace genie
