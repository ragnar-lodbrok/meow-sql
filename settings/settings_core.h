#ifndef MEOW_SETTINGS_CORE_H
#define MEOW_SETTINGS_CORE_H

#include "settings_text.h"
#include "settings_geometry.h"

namespace meow {
namespace settings {

class Core
{
public:
    Core();

    Text * textSettings() { return &_settingsText; }
    Geometry * geometrySettings() { return &_geometry; }

private:
    Text _settingsText;
    Geometry _geometry;
};

} // namespace meow
} // namespace settings


#endif // MEOW_SETTINGS_CORE_H
