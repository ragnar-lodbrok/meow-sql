#include <QApplication>
#include "app.h"
#include "ui/main_window/main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath(QCoreApplication::applicationDirPath() + "/imageformats");

    QCoreApplication::setOrganizationName("ragnar-lodbrok");
    QCoreApplication::setApplicationName("MeowSQL");

    meow::App app;

    meow::ui::main_window::Window w;
    w.show();

    w.showSessionManagerDialog();

    return a.exec();
}
