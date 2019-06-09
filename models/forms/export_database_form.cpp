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

} // namespace forms
} // namespace models
} // namespace meow

