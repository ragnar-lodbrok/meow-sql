#include "query_data_exporter.h"
#include "query_data_export_formats/format_factory.h"

#include <QTextCodec>


namespace meow {
namespace utils {
namespace exporting {

QueryDataExporter::QueryDataExporter()
{
    _encoding = defaultFileEncoding();

    QueryDataExportFormatFactory formatFactory;
    _formats = formatFactory.createFormats();
}

QStringList QueryDataExporter::supportedFileEncodings() const
{
    // TODO: cache?
    QList<QByteArray> codecs = QTextCodec::availableCodecs();

    QStringList codecNames;

    for (const QByteArray & codec : codecs) {
        QString name = QString::fromUtf8(codec);
        if (!codecNames.contains(name)) {
            codecNames.push_back(name);
        }
    }

    return codecNames;
}

QStringList QueryDataExporter::formatNames() const
{
    QStringList names;

    for (const QueryDataExportFormatPtr & format : _formats) {
        names << format->name();
    }

    return names;
}

} // namespace exporting
} // namespace utils
} // namespace meow
