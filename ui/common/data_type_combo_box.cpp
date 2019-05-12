#include "data_type_combo_box.h"

namespace meow {
namespace ui {

DataTypeComboBox::DataTypeComboBox(QWidget * parent) : QComboBox(parent)
{
    setMaxVisibleItems(30);
}

void DataTypeComboBox::fillData(const QStringList & names)
{

    for (int i=0; i<names.size(); ++i) { // TODO: vector is faster?
        addItem(names[i], i);
    }
}

} // namespace ui
} // namespace meow
