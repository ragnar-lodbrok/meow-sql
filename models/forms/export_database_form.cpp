#include "export_database_form.h"
#include "db/connection.h"
#include "db/entity/session_entity.h"

#include <QStandardPaths>
#include <QDir>

namespace meow {
namespace models {
namespace forms {

ExportDatabaseForm::ExportDatabaseForm(db::SessionEntity * session)
    : _session(session)
    , _allDatabases(true)
    , _filename(generateFilename())
{

}

const QStringList ExportDatabaseForm::databases() const
{
    QStringList list;
    list << QObject::tr("All databases");
    list << _session->connection()->allDatabases();
    return list;
}

QString ExportDatabaseForm::generateFilename() const
{
    // TODO: more advanced
    return QDir::toNativeSeparators(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + QDir::separator() + "dump.sql"
    );
}

void ExportDatabaseForm::startExport()
{
    _dumper.reset(new meow::utils::exporting::MySQLDumpConsole(this));

    connect(_dumper.get(),
            &meow::utils::exporting::MySQLDumpConsole::finished,
            this,
            &ExportDatabaseForm::finished);

    connect(_dumper.get(),
            &meow::utils::exporting::MySQLDumpConsole::progressMessage,
            this,
            &ExportDatabaseForm::progressMessage);

    _dumper->start();
}

void ExportDatabaseForm::cancelExport()
{
    if (_dumper) {
        _dumper->cancel();
    }
}

} // namespace forms
} // namespace models
} // namespace meow

