#include "settings_text.h"
#include <algorithm>
#include <QMapIterator>

namespace meow {
namespace settings {

Text::Text()
{
    setupColorForDataTypes();
}

void  Text::setupColorForDataTypes()
{
    _colorsDataTypes[db::DataTypeCategoryIndex::Integer]  = QColor(0,  0, 255);
    _colorsDataTypes[db::DataTypeCategoryIndex::Float]    = QColor(72, 0, 255);
    _colorsDataTypes[db::DataTypeCategoryIndex::Text]     = QColor(0, 128, 0);
    _colorsDataTypes[db::DataTypeCategoryIndex::Binary]   = QColor(128, 0, 128);
    _colorsDataTypes[db::DataTypeCategoryIndex::Temporal] = QColor(128, 0, 255);
    _colorsDataTypes[db::DataTypeCategoryIndex::Spatial]  = QColor(0, 128, 128);
    _colorsDataTypes[db::DataTypeCategoryIndex::Other]    = QColor(128, 128, 0);

    const int COLORSHIFT_NULLFIELDS = 70;

    QMapIterator<db::DataTypeCategoryIndex, QColor> i(_colorsDataTypes);
    while (i.hasNext()) {
        i.next();
        QColor hls = i.value().toHsl();
        int l = hls.lightness() + COLORSHIFT_NULLFIELDS;
        int s = std::max(0, hls.saturation() - 2*COLORSHIFT_NULLFIELDS);
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


} // namespace meow
} // namespace settings
