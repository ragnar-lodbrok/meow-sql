#include <QApplication>
#include "app/app.h"
#include "ui/main_window/main_window.h"

#ifdef GENERATED_BY_CMAKE
#include "meowsql_config.h"
#endif

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

    QCoreApplication::setOrganizationName("ragnar-lodbrok"); // TODO: change to MeowSQL
    QCoreApplication::setApplicationName("MeowSQL");
#ifdef meowsql_VERSION_MAJOR
    QCoreApplication::setApplicationVersion(
        QString("%1.%2.%3").arg(QString::number(meowsql_VERSION_MAJOR),
                                QString::number(meowsql_VERSION_MINOR),
                                QString::number(meowsql_VERSION_PATCH))
    );
#endif

    meow::App app;

    QString language = app.settings()->generalSettings()->language();
    QTranslator translator;
    if (translator.load(QString(":translations/meowsql_%1.qm").arg(language))) {
        a.installTranslator(&translator);
    }

    meow::ui::main_window::Window w;
    w.show();

    w.showSessionManagerDialog();

    return a.exec();
}
