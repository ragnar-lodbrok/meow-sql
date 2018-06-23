#ifndef MEOW_SETTINGS_TEXT_H
#define MEOW_SETTINGS_TEXT_H

#include "db/data_type/data_type_category.h"
#include <QColor>
#include <QMap>

namespace meow {

namespace db {
    enum class ColumnDefaultType;
}

namespace settings {

class Text
{
public:
    Text();

    int tableAutoResizeRowsLookupCount() const { return 10; }
    bool autoResizeTableColumns() const { return true; }
    bool autoLimitLoadDataLength() const { return true; }

    const QColor colorForDataType(db::DataTypeCategoryIndex type) const;
    const QColor colorForDataTypeNULL(db::DataTypeCategoryIndex type) const;
    const QColor colorForDefaultType(db::ColumnDefaultType type) const;
private:

    void setupColorForDataTypes();

    QMap<db::DataTypeCategoryIndex, QColor> _colorsDataTypes;
    QMap<db::DataTypeCategoryIndex, QColor> _colorsDataTypesNULL;
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_TEXT_H
