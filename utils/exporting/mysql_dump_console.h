#ifndef UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H
#define UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H

#include <QProcess>
#include <memory>

namespace meow {

namespace models {
namespace forms {

class ExportDatabaseForm;

}
}

namespace utils {
namespace exporting {

class MySQLDumpConsole : public QObject
{
    Q_OBJECT

public:
    MySQLDumpConsole(models::forms::ExportDatabaseForm * form);

    ~MySQLDumpConsole();

    void start();
    bool cancel();

    bool isRunning() const;

    QString version() const;
    QString currentCommand() const;

    Q_SIGNAL void finished(bool success);
    Q_SIGNAL void progressMessage(const QString & str);

private:

    QStringList programArguments() const;

    QStringList connectionOptions() const;

    bool enterPasswordIfRequested(const QString & output);

    QString pathToCommand() const;

    std::unique_ptr<QProcess> _process;

    models::forms::ExportDatabaseForm * _form;

    bool _passwordEntered;
    bool _isCancelledbyUser;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H
