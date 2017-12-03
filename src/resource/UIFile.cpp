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


#include "genie/resource/UIFile.h"

#include <iostream>
#include <stdexcept>

#include "genie/util/Logger.h"

namespace genie
{

using std::string;

Logger& UIFile::log = Logger::getLogger("genie.UIFile");

//------------------------------------------------------------------------------
UIFile::UIFile()
{
}

//------------------------------------------------------------------------------
UIFile::~UIFile()
{
}

//------------------------------------------------------------------------------
void UIFile::serializeObject(void)
{
  if (isOperation(OP_READ))
  {
    std::istream *istr = getIStream();

    if (!readBackground(istr, &background1, "background1_files")) {
        return;
    }
    if (!readBackground(istr, &background2, "background2_files")) {
        return;
    }
    if (!readBackground(istr, &background3, "background3_files")) {
        return;
    }
    if (!readFileReference(istr, &paletteFile, "palette_file")) {
        return;
    }
    if (!readFileReference(istr, &cursorFile, "cursor_file")) {
        return;
    }
    if (!readValue(istr, &shadePercent, "shade_amount", "percent")) {
        return;
    }
    if (!readFileReference(istr, &buttonFile, "button_file")) {
        return;
    }
    if (!readFileReference(istr, &popupDialogFile, "popup_dialog_sin")) {
        return;
    }

    if (!readValue(istr, &backgroundPosition, "background_position")) {
        return;
    }
    if (!readValue(istr, &backgroundColor, "background_color")) {
        return;
    }

    std::string bevelColorName;
    *istr >> bevelColorName;
    if (bevelColorName != "bevel_colors") {
        log.error("Expected name bevel_colors, got %s", bevelColorName.c_str());
        return;
    }

    // istream doesn't understand uint8_t
    int bevelColor1r, bevelColor1g, bevelColor1b;
    *istr >> bevelColor1r >> bevelColor1g >> bevelColor1b;
    bevelColor1.r = bevelColor1r;
    bevelColor1.g = bevelColor1g;
    bevelColor1.b = bevelColor1b;

    int bevelColor2r, bevelColor2g, bevelColor2b;
    *istr >> bevelColor2r >> bevelColor2g >> bevelColor2b;
    bevelColor2.r = bevelColor2r;
    bevelColor2.g = bevelColor2g;
    bevelColor2.b = bevelColor2b;

    if (!readColor(istr, &textColor1, "text_color1")) {
        return;
    }
    if (!readColor(istr, &textColor2, "text_color2")) {
        return;
    }
    if (!readColor(istr, &focusColor1, "focus_color1")) {
        return;
    }
    if (!readColor(istr, &focusColor2, "focus_color2")) {
        return;
    }
    if (!readColor(istr, &stateColor1, "state_color1")) {
        return;
    }
    if (!readColor(istr, &stateColor2, "state_color2")) {
        return;
    }
  }
  else
  {
      log.error("writing not implemented");
      return;
  }
}

bool UIFile::readBackground(std::istream *istr, UIFile::Background *background, const std::string &expectedName)
{
    std::string name;
    *istr >> name;
    if (name != expectedName) {
        log.error("Expected name %s, got %s", expectedName.c_str(), name.c_str());
        return false;
    }

    *istr >> background->category;
    *istr >> background->unknown_string;
    *istr >> background->imageId;
    *istr >> background->unknown_number;

    return true;
}

bool UIFile::readFileReference(std::istream *istr, UIFile::FileReference *fileReference, const std::string &expectedName)
{
    std::string name;
    *istr >> name;
    if (name != expectedName) {
        log.error("Expected name %s, got %s!", expectedName.c_str(), name.c_str());
        return false;
    }
    *istr >> fileReference->category;
    *istr >> fileReference->id;
    return true;
}

bool UIFile::readValue(std::istream *istr, uint32_t *val, const std::string &expectedName, const std::string &expectedType)
{
    std::string name;
    *istr >> name;
    if (name != expectedName) {
        log.error("Expected name %s, got %s", expectedName.c_str(), name.c_str());
        return false;
    }
    if (!expectedType.empty()) {
        std::string type;
        *istr >> type;
        if (type != expectedType) {
            log.warn("Expected type %s, got %s", expectedType.c_str(), type.c_str());
        }
    }

    *istr >> *val;

    return true;
}

bool UIFile::readColor(std::istream *istr, Color *color, const std::string &expectedName)
{
    std::string name;
    *istr >> name;
    if (name != expectedName) {
        log.error("Expected name %s, got %s", expectedName.c_str(), name.c_str());
        return false;
    }

    int colorR, colorG, colorB;
    *istr >> colorR >> colorG >> colorB;
    color->r = colorR;
    color->g = colorG;
    color->b = colorB;

    return true;
}


}


