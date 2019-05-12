#ifndef DATA_TYPE_COMBO_BOX_H
#define DATA_TYPE_COMBO_BOX_H

#include <QComboBox>

namespace meow {
namespace ui {

class DataTypeComboBox : public QComboBox
{
public:
    DataTypeComboBox(QWidget *parent = nullptr);

    void fillData(const QStringList & names);
};

} // namespace ui
} // namespace meow

#endif // DATA_TYPE_COMBO_BOX_H
