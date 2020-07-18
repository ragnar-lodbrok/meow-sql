#include "settings_text.h"
#include <algorithm>
#include <QMapIterator>
#include <QGuiApplication>
#include <QPalette>
#include "db/table_column.h"

namespace meow {
namespace settings {

static inline bool isLightTheme()
{
    QColor textColor = QGuiApplication::palette().color(QPalette::WindowText);

    // very stupid way to distinguish dark and light themes
    return (textColor.redF() + textColor.greenF() + textColor.blueF()) < 0.5*3;
}

Text::Text()
{
    setupColorForDataTypes();
}

void Text::setupColorForDataTypes()
{

    if (isLightTheme()) {
        _colorsDataTypes[db::DataTypeCategoryIndex::Integer]  = QColor(0,  0, 255);
        _colorsDataTypes[db::DataTypeCategoryIndex::Float]    = QColor(72, 0, 255);
        _colorsDataTypes[db::DataTypeCategoryIndex::Text]     = QColor(0, 128, 0);
        _colorsDataTypes[db::DataTypeCategoryIndex::Binary]   = QColor(128, 0, 128);
        _colorsDataTypes[db::DataTypeCategoryIndex::Temporal] = QColor(128, 0, 0);
        _colorsDataTypes[db::DataTypeCategoryIndex::Spatial]  = QColor(0, 128, 128);
        _colorsDataTypes[db::DataTypeCategoryIndex::Other]    = QColor(128, 128, 0);
    } else {
        _colorsDataTypes[db::DataTypeCategoryIndex::Integer]  = QColor(133, 151, 255);
        _colorsDataTypes[db::DataTypeCategoryIndex::Float]    = QColor(125, 125, 208);
        _colorsDataTypes[db::DataTypeCategoryIndex::Text]     = QColor(115, 213, 115);
        _colorsDataTypes[db::DataTypeCategoryIndex::Binary]   = QColor(127, 118, 201);
        _colorsDataTypes[db::DataTypeCategoryIndex::Temporal] = QColor(201, 115, 115);
        _colorsDataTypes[db::DataTypeCategoryIndex::Spatial]  = QColor(115, 206, 206);
        _colorsDataTypes[db::DataTypeCategoryIndex::Other]    = QColor(193, 193, 115);
    }

    const int COLORSHIFT_NULLFIELDS = 70;

    QMapIterator<db::DataTypeCategoryIndex, QColor> i(_colorsDataTypes);
    while (i.hasNext()) {
        i.next();
        QColor hls = i.value().toHsl();
        int l = 0;
        int s = 0;
        if (isLightTheme()) {
            l = hls.lightness() + COLORSHIFT_NULLFIELDS;
            s = std::max(0, hls.saturation() - 2*COLORSHIFT_NULLFIELDS);
        } else {
            l = hls.lightness() - COLORSHIFT_NULLFIELDS;
            s = std::max(0, hls.saturation() - COLORSHIFT_NULLFIELDS/2);
        }
        hls.setHsl(hls.hue(), l, s);
        _colorsDataTypesNULL[i.key()] = hls.toRgb();
    }
}

const QColor Text::colorForDataType(db::DataTypeCategoryIndex type) const
{
    return _colorsDataTypes.value(type);
}


const QColor Text::colorForDataTypeNULL(db::DataTypeCategoryIndex type) const
{
    return _colorsDataTypesNULL.value(type);
}

const QColor Text::colorForDefaultType(db::ColumnDefaultType type) const
{
    switch (type) {
    case db::ColumnDefaultType::None:
    case db::ColumnDefaultType::Null:
    case db::ColumnDefaultType::NullUpdateTS:
        return isLightTheme() ? QColor(100, 100, 100) : QColor(200, 200, 200);

    case db::ColumnDefaultType::CurTS:
    case db::ColumnDefaultType::CurTSUpdateTS:
        return colorForDataType(db::DataTypeCategoryIndex::Temporal);

    case db::ColumnDefaultType::AutoInc:
        return isLightTheme() ? QColor(0, 0, 128) : QColor(66, 75, 255);

    default:
        break;
    }

    return isLightTheme() ? QColor(0, 0, 0) : QColor(255, 255, 255);
}

} // namespace meow
} // namespace settings
