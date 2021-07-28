/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml

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

#include "genie/resource/PalFile.h"

#include <iostream>
#include <stdexcept>

#include <string_view>

#include "genie/util/Logger.h"

namespace genie {

// I don't know colors, so don't trust the naming etc.
constexpr inline Color hsvToColor(int hue, int saturation, int value)
{
    if(hue <= -1 || saturation > 255 || value > 255 || hue >= 360) {
        return Color(255, 0, 0);
    }


    // Find which primary color it is (red, yellow, blue, whatever)/part of the circle
    const int primary = hue / 60;
    const uint32_t blend = hue % 60; // how far into the primary are we, ish

    const uint32_t chroma = (2 * value * (255-saturation) + 255) / 510;

    if (primary & 1){
        const uint32_t secondary = (2 * value * (15300 - saturation * blend) + 15300) / 30600;
        switch (primary) {
        case 1:
            return Color(secondary, value, chroma);
        case 3:
            return Color(chroma, secondary, value);
        case 5:
            return Color(value, chroma, secondary);
        default:
            break;
        }
    } else {
        const uint32_t secondary = (2 * value * (15300 - (saturation * (60 - blend))) + 15300) /30600;
        switch(primary) {
        case 0:
            return Color(value, secondary, chroma);
        case 2:
            return Color(chroma, value, secondary);
        case 4:
            return Color(secondary, chroma, value);
        default:
            break;
        }
    }
    return Color(255, 0, 0);
}

struct DefaultPalette {
    static constexpr int black = 0;
    static constexpr int black2 = 10;
    static constexpr int black4 = 206;


    static constexpr int white = 255;
    static constexpr int fullGreen = 241;
    static constexpr int fullBlue = 242;
    static constexpr int fullYellow = 242;
    static constexpr int darkYellow = 242;

    static constexpr int blueTeam = 1;
    static constexpr int redTeam = 2;
    static constexpr int greenTeam = 3;
    static constexpr int yellowTeam = 4;
    static constexpr int orangeTeam = 5;
    static constexpr int tealTeam = 6;
    static constexpr int pinkTeam = 7;
    static constexpr int grayTeam = 8;

    // THese colors are eyeballed/guessed and a bit off, they are not the same as in the original palette
    // In general I try to keep the saturation low, because that hides some of the most glaring mismatches
    // You have been warned
    // TODO: make constexpr when microsoft fixes their compilator
    /*constexpr*/ DefaultPalette() {
        // Fallback in case I forgot some
        for (int i=0; i<256; i++) {
            colors[i] = Color(i, i, i);
        }

        colors[black] = {0, 0, 0};

        colors[white] = {255, 255, 255};
        for (int i=245; i<249; i++) {
            colors[i] = colors[white];
        }

        colors[fullGreen] = {0, 255, 0};
        colors[fullBlue] = {0, 0, 255};
        colors[fullYellow] = {255, 255, 0};
        colors[darkYellow] = {192, 192, 0};

        // Generate something that looks ish like the normal team colors
        for (int team=1; team<=8; team++) {
            int hue = 0;
            int saturation = 64;
            switch(team) {
            case blueTeam:
                hue = 240;
                break;
            case redTeam:
                hue = 0;
                break;
            case greenTeam:
                hue = 120;
                break;
            case yellowTeam:
                hue = 60;
                break;
            case orangeTeam:
                hue = 30;
                break;
            case tealTeam:
                hue = 180;
                break;
            case pinkTeam:
                hue = 300;
                break;
            case grayTeam:
                saturation = 0;
                break;
            }

            for (int color=0; color<8; color++) {
                saturation *= 1.1;
                colors[team * 16 + color] = hsvToColor(hue, saturation, color * 16 + 70);
            }
        }

        // blue variation
        for (int i=1; i<8; i++) {
            colors[i] = Color(32, 32, i*8 + 191);
            colors[i + 24] = Color(0, 0, i);
        }

        // Some brown stuff
        for (int team=0; team<=8; team++) {
            const int hue = 25+team;
            const int saturation = team*5 + 80;
            for (int color=0; color<8; color++) {
                const int value = team * 31 + color;
                colors[team * 16 + color + 8] = hsvToColor(hue, saturation, value);
            }
        }

        colors[8] = Color(128, 128, 255); // greenish?
        colors[9] = Color(128, 255, 128); // blue?
        colors[10] = colors[black];

        // Some greenish
        for (int i=144; i<160; i++) {
            const int hue = 60;
            const int saturation = 255 - i * 8;
            const int value = (i - 128) * 4;
            colors[i] = hsvToColor(hue, saturation, value);
        }

        // light-greenish, can't be assed to find some of the correct variation
        for (int i=160; i<176; i++) {
//            colors[i] = Color(255, 0, 0);
            colors[i] = hsvToColor(60, 50, 150);
        }

        // light-blue-ish?
        colors[192] = colors[240] = hsvToColor(200, 90, 240);

        // green variation
        for (int i=177; i<192; i++) {
//            const int value = (i - 191) * 8;
//            colors[i] = convertHSV2RGB(120, 120, value);
            const int value = i+10;
            colors[i] = hsvToColor(120, 94, value/2);

        }

        // green variation 2
        for (int i=193; i<198; i++) {
            colors[i] = Color(64, i/2, 64);
            colors[i + 32] = Color(0, i, 0);
        }
        // blue variation 2
        for (int i=198; i<208; i++) {
            colors[i] = Color(0, 0, i);
        }
        // blue variation 3
        for (int i=208; i<232; i++) {
            colors[i] = hsvToColor(230, 50, i);;
        }

        // green variation 3
        for (int i=233; i<240; i++) {
            const int value = i+50;
            colors[i] = hsvToColor(120, 94, value/2);

        }

        colors[208] = Color(64, 128, 64); // dark green?
        colors[209] = Color(128, 128, 255); // light blue-ish?
        colors[210] = colors[211] = colors[214] = Color(0, 0, 64); // darke blue
        colors[213] =  Color(0, 0, 128); // lighter blue
        colors[215] = colors[217] = colors[209]; // lighter blue
        colors[249] = colors[250] = Color(32, 128, 32);
        colors[13] = colors[56] = colors[94] = colors[182] = colors[218] = Color(92, 92, 92); // gray

        colors[black2] = colors[black];
        colors[black4] = colors[black];

        colors[251] = hsvToColor(180, 255, 255); // teal
        colors[252] = hsvToColor(300, 255, 255); // pink

        colors[253] = colors[254] = Color(128, 128, 192); // mid blue

        // orange-ish?
        colors[176] = colors[212] = colors[229] = hsvToColor(40, 192, 128);
    }
    std::array<Color, 256> colors;
};

const PalFile PalFile::null;

using std::string;

Logger &PalFile::log = Logger::getLogger("genie.PalFile");

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const std::vector<Color> &PalFile::getColors() const
{
    static const DefaultPalette fallback;
    static const std::vector<Color> fallbackColors(fallback.colors.begin(), fallback.colors.end());
    if (colors_.empty()) {
        return fallbackColors;
    }
    return colors_;
}

//------------------------------------------------------------------------------
size_t PalFile::size() const
{
    return colors_.size();
}

//------------------------------------------------------------------------------
void PalFile::serializeObject()
{
    if (isOperation(OP_READ)) {
        std::istream *istr = getIStream();

        std::string header(fileHeader.size(), 0);

        istr->read(header.data(), header.size());

        if (header != fileHeader) {
            log.error("Not a color palette!");
            //TODO: Exception
            return;
        }

        std::string versionHeader(fileVersionHeader.size(), 0);
        istr->read(versionHeader.data(), versionHeader.size());

        if (versionHeader != fileVersionHeader) {
            log.warn("Invalid file version %", versionHeader);
        }

        *istr >> num_colors_;
        log.debug("% colors", num_colors_);

        for (uint32_t i = 0; i < num_colors_; i++) {
            uint32_t r, g, b, a = 255;
            *istr >> r >> g >> b;

            // aoede and aoe2de have alpha, TODO: set and use gameversion
            if (istr->peek() == ' ') {
                *istr >> a;
            }

            colors_.emplace_back(r, g, b, a);
        }
    } else {
        std::ostream *ostr = getOStream();

        static const std::string lineEnding("\r\n"); // windows line ending

        *ostr << fileHeader;
        *ostr << fileVersionHeader;

        if (colors_.size() > 256) {
            log.error("Too much colors (>256)");
        }

        *ostr << colors_.size() << lineEnding;

        for (uint32_t i = 0; i < colors_.size(); i++) {
            *ostr << (int)colors_[i].r << " ";
            *ostr << (int)colors_[i].g << " ";
            *ostr << (int)colors_[i].b << lineEnding;
        }
    }
}

//------------------------------------------------------------------------------
size_t PalFile::numOfChars(uint8_t number)
{
    if (number < 10) {
        return 1;
    }

    if (number < 100) {
        return 2;
    }

    return 3;
}

//------------------------------------------------------------------------------
size_t PalFile::objectSize()
{
    size_t size = 0;

    constexpr size_t lineEndingSize = std::string_view("\r\n").size();

    size += fileHeader.size();
    size += fileVersionHeader.size();

    if (colors_.size() > 256) {
        log.error("Too much colors (>256)");
    }

    size += numOfChars(colors_.size()) + lineEndingSize;

    for (uint32_t i = 0; i < colors_.size(); i++) {
        size += numOfChars(colors_[i].r);
        size += numOfChars(colors_[i].g);
        size += numOfChars(colors_[i].b);

        size += 2; // spaces
        size += lineEndingSize;
    }

    return size;
}

bool PalFile::isValid() const
{
    return !colors_.empty();
}

} // namespace genie
