#pragma once

#include "genie/file/ISerializable.h"
#include "genie/util/Logger.h"

namespace genie {

class SmpFrame : public ISerializable
{
    static Logger &log;
};

}//namespace genie
