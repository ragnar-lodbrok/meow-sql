#ifndef UI_CHECKBOX_LIST_POPUP_H
#define UI_CHECKBOX_LIST_POPUP_H

#include <QtWidgets>

namespace meow {
namespace ui {

class CheckboxListPopup : public QFrame
{
public:
    explicit CheckboxListPopup(QWidget * parent = nullptr);
    void setData(const QStringList & allOptions,
                 const QStringList & selectedOptions);
    QStringList selectedOptions() const;

    int heightHint() const;
    int widthHint() const;

private:
    QListWidget * _listWidget;
};

} // namespace ui
} // namespace meow


#endif // UI_CHECKBOX_LIST_POPUP_H
