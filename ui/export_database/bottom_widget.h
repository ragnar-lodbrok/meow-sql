#ifndef UI_EXPORT_DATABASE_BOTTOM_WIDGET_H
#define UI_EXPORT_DATABASE_BOTTOM_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace export_database {

class BottomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BottomWidget(QWidget *parent = nullptr);

    QPushButton * exportButton() const { return _exportButton; }
    QPushButton * cancelButton() const { return _cancelButton; }

private:
    void createWidgets();

    QPushButton * _exportButton;
    QPushButton * _cancelButton;

};

} // namespace export_database
} // namespace ui
} // namespace meow


#endif // UI_EXPORT_DATABASE_BOTTOM_WIDGET_H
