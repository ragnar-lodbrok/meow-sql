#ifndef UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H
#define UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H

#include <QProcess>
#include <QVersionNumber>
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
    explicit MySQLDumpConsole(models::forms::ExportDatabaseForm * form);

    ~MySQLDumpConsole();

    void start();
    bool cancel();

    bool isRunning() const;

    QString versionString() const;
    QVersionNumber versionNumber() const;
    QString currentCommand() const;

    bool supportsColumnStatisticsOption() const;

    Q_SIGNAL void finished(bool success);
    Q_SIGNAL void progressMessage(const QString & str);

private:

    QStringList programArguments() const;

    QStringList connectionOptions() const;

    bool enterPasswordIfRequested(const QString & output);

    QString pathToCommand() const;

    QString password() const;
    QString escapeString(const QString & str) const;

    std::unique_ptr<QProcess> _process;

    models::forms::ExportDatabaseForm * _form;

    bool _passwordEntered;
    bool _isCancelledbyUser;
    mutable QString _versionString;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // UTILS_EXPORTING_MYSQL_DUMP_CONSOLE_H
