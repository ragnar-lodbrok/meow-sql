#ifndef MEOW_SETTINGS_CORE_H
#define MEOW_SETTINGS_CORE_H

#include "settings_text.h"
#include "settings_geometry.h"
#include "settings_icons.h"
#include "data_editors.h"
#include "queries_storage.h"
#include "general.h"

#include <memory>

namespace meow {
namespace settings {

class UserPreferences
{
public:
    General * generalSettings() { return &_general; }
    std::unique_ptr<UserPreferences> clone() const {
        auto copy = std::unique_ptr<UserPreferences>(new UserPreferences);
        _general.copyDataTo(copy->generalSettings());
        return copy;
    }

    void load() {
        _general.load();
    }

    void save() {
        _general.save();
    }

    void setDataFrom(UserPreferences * source) {
        _general.setDataFrom(source->generalSettings());
    }

private:
    General _general;
};

class Core
{
public:
    Core();

    Core(const Core&) = delete;
    Core(Core&&) = delete;
    Core& operator=(const Core&) = delete;
    Core& operator=(Core&&) = delete;

    UserPreferences * userPreferences() { return &_userPreferences; }

    General * generalSettings() { return _userPreferences.generalSettings(); }

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
    UserPreferences _userPreferences;
    Text _text;
    Geometry _geometry;
    Icons _icons;
    DataEditors _dataEditors;
    std::unique_ptr<QueriesStorage> _queriesStorage;
};

} // namespace meow
} // namespace settings


#endif // MEOW_SETTINGS_CORE_H
