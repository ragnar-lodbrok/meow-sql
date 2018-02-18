#include "data_type_combo_box.h"
#include "db/data_type/data_type.h"

namespace meow {
namespace ui {

DataTypeComboBox::DataTypeComboBox(QWidget * parent) : QComboBox(parent)
{
    setMaxVisibleItems(30);
}

void DataTypeComboBox::fillData()
{
    // This is very stupid and basic, better use model
    // TODO: subtypes, colors, hints like in Heidi
    QMap<meow::db::DataTypeIndex, QString> types = meow::db::dataTypeNames();

    auto it = types.constBegin();
    while (it != types.constEnd()) {
        QString name = it.value();
        int key = static_cast<int>(it.key());

        addItem(name, key);

        ++it;
    }
}

} // namespace ui
} // namespace meow
