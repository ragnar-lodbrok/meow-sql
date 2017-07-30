#ifndef MEOW_SETTINGS_CORE_H
#define MEOW_SETTINGS_CORE_H

#include "settings_text.h"

namespace meow {
namespace settings {

class Core
{
public:
    Core();

    Text * textSettings() { return &_settingsText; }

private:
    Text _settingsText;
};

} // namespace meow
} // namespace settings


#endif // MEOW_SETTINGS_CORE_H
