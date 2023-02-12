#include "settings_core.h"

namespace meow {
namespace settings {

Core::Core()
{
    _userPreferences.load();
}

} // namespace meow
} // namespace settings
