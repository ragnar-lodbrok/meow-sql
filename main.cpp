#include <QApplication>
#include "app/app.h"
#include "ui/main_window/main_window.h"

#ifdef Q_OS_WIN
    #include <Windows.h> // SetProcessDPIAware()
#endif // Q_OS_WIN

int main(int argc, char *argv[])
{

#ifdef Q_OS_WIN
    // fix scaling issues on win (TODO: rm when upgrade from Qt 5.6 ?)
    ::SetProcessDPIAware();
#endif // Q_OS_WIN

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // before app!

    QApplication a(argc, argv);
    a.addLibraryPath(QCoreApplication::applicationDirPath() + "/imageformats");

    QCoreApplication::setOrganizationName("ragnar-lodbrok");
    QCoreApplication::setApplicationName("MeowSQL");


    meow::App app;

    meow::ui::main_window::Window w;
    
    w.showSessionManagerDialog();

    return a.exec();
}
