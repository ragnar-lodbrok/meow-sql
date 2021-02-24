#ifndef UI_CENTRAL_RIGHT_BASEROOTTAB_H
#define UI_CENTRAL_RIGHT_BASEROOTTAB_H

#include <QWidget>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class BaseRootTab : public QWidget
{
public:

    enum class Type
    {
        Host,
        DataBase,
        Entity,
        Data,
        Query,
        AddQuery
    };

    explicit BaseRootTab(Type type, QWidget * parent = nullptr)
        : QWidget(parent)
        , _type(type)
    {

    }

    Type tabType() const {
        return _type;
    }
private:
    Type _type;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_BASEROOTTAB_H
