#ifndef MEOW_SETTINGS_ICONS_H
#define MEOW_SETTINGS_ICONS_H

#include <QIcon>
#include <QMap>

namespace meow {

namespace db {
    enum class TableIndexClass;
}

namespace settings {

class Icons
{
public:
    Icons();

    const QIcon iconForTableIndexClass(db::TableIndexClass indexClass) const;

private:

    void setupIcons();

    QMap<db::TableIndexClass, QIcon> _iconsTableIndexClass;
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_ICONS_H
