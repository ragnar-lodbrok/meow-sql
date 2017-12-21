#ifndef MEOW_SETTINGS_CORE_H
#define MEOW_SETTINGS_CORE_H

#include "settings_text.h"
#include "settings_geometry.h"
#include "settings_icons.h"

namespace meow {
namespace settings {

class Core
{
public:
    Core();

    Text * textSettings() { return &_text; }
    Geometry * geometrySettings() { return &_geometry; }
    Icons * icons() { return &_icons; }

private:
    Text _text;
    Geometry _geometry;
    Icons _icons;
};

} // namespace meow
} // namespace settings


#endif // MEOW_SETTINGS_CORE_H
