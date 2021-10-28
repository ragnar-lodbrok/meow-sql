#ifndef MEOW_SETTINGS_CORE_H
#define MEOW_SETTINGS_CORE_H

#include "settings_text.h"
#include "settings_geometry.h"
#include "settings_icons.h"
#include "data_editors.h"
#include "queries_storage.h"

#include <memory>

namespace meow {
namespace settings {

class Core
{
public:
    Core();
    Text * textSettings() { return &_text; }
    Geometry * geometrySettings() { return &_geometry; }
    Icons * icons() { return &_icons; }
    DataEditors * dataEditors() { return &_dataEditors; }
    QueriesStorage * queriesStorage() {
        if (_queriesStorage == nullptr) {
            _queriesStorage.reset(new QueriesStorage());
        }
        return _queriesStorage.get();
    }
private:
    Text _text;
    Geometry _geometry;
    Icons _icons;
    DataEditors _dataEditors;
    std::unique_ptr<QueriesStorage> _queriesStorage;
};

} // namespace meow
} // namespace settings


#endif // MEOW_SETTINGS_CORE_H
