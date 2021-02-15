#ifndef SESSIONMANAGERWINDOW_H
#define SESSIONMANAGERWINDOW_H

#include <QtWidgets>
#include <QSharedPointer>

#include "ui/session_manager/session_form.h"
#include "models/db/connection_params_model.h"

namespace meow {
namespace ui {

namespace main_window {
    class Window;
}

namespace session_manager {

class Window : public QDialog
{
    Q_OBJECT

public:
    explicit Window(meow::ui::main_window::Window * mainWindow);
    ~Window() override;

private:

    main_window::Window * _mainWindow;

    // main layout
    QHBoxLayout * _mainLayout;
    QSplitter   * _mainSplitter;
    QWidget     * _mainLeftWidget;
    QWidget     * _mainRightWidget;
    // left widget
    QVBoxLayout * _leftWidgetMainLayout;
    QTableView  * _sessionsList;
    QHBoxLayout * _leftWidgetButtonsLayout;
    QPushButton * _newSessionButton;
    QPushButton * _saveSessionButton;
    QPushButton * _deleteSessionButton;
    // right widget
    QVBoxLayout * _rightWidgetMainLayout;
    SessionForm * _sessionDetails;
    QHBoxLayout * _rightWidgetButtonsLayout;
    QPushButton * _openSessionButton;
    QPushButton * _cancelButton;
    QPushButton * _moreButton;

    void createMainLayout();
    // left widget
    void createLeftSubWidgets();
    void createSessionsList();
    void createLeftWidgetButtons();
    // right widget
    void createRightSubWidgets();
    void createRightWidgetButtons();

    Q_SLOT void currentSessionDetailsChanged(const QItemSelection &selected,
                                             const QItemSelection &deselected);

    Q_SLOT void saveCurrentSession();
    Q_SLOT void createNewSession();
    Q_SLOT void deleteCurrentSession();
    Q_SLOT void openCurrentSession();
    void validateControls();
    bool finalizeCurModifications();
    void selectSessionAt(int rowIndex);

    void showErrorMessage(const QString& message);

    void saveGeometryToSettings();
    void loadGeometryFromSettings();

    QSharedPointer<meow::models::db::ConnectionParamsModel> _connectionParamsModel;
    //QSortFilterProxyModel _proxySortModel;// TODO

    bool _saveConfirmCanceled;

}; // class Window

} // namespace session_manager
} // namespace ui
} // namespace meow

#endif // SESSIONMANAGERWINDOW_H
