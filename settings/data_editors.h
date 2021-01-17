#ifndef MEOW_SETTINGS_DATA_EDITORS_H
#define MEOW_SETTINGS_DATA_EDITORS_H

namespace meow {
namespace settings {

class DataEditors
{
public:
    DataEditors();
    bool enableInplaceDatetimeEditor() const { return true; }
    bool prefillDatetimeEditorWithCurrent() const { return true; }
    bool enablePopupForDatetimeEditor() const { return false; }
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_DATA_EDITORS_H
