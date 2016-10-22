#include <QApplication>
#include "app.h"
#include "mainwindow.h"
#include "ui/session_manager/window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("ragnar-lodbrok");
    QCoreApplication::setApplicationName("MeowSQL");

    meow::App app;

    MainWindow w;
    w.show();

    meow::ui::session_manager::Window sessionManagerWindow(&w);
    sessionManagerWindow.show();

    return a.exec();
}
