#include "query_data_exporter.h"
#include "query_data_export_formats/format_factory.h"
#include "ui/models/base_data_table_model.h"

#include <QTextCodec>


namespace meow {
namespace utils {
namespace exporting {

QueryDataExporter::QueryDataExporter()
    : _encoding(defaultFileEncoding())
{

    QueryDataExportFormatFactory formatFactory;
    std::vector<QueryDataExportFormatPtr> formats
            = formatFactory.createFormats();
    for (const QueryDataExportFormatPtr & format : formats) {
        _formats.insert(format->id(), format);
    }
}

void QueryDataExporter::setData(
        ui::models::BaseDataTableModel * model,
        QItemSelectionModel * selection)
{
    _model = model;
    _selection = selection;
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

QMap<QString, QString> QueryDataExporter::formatNames() const
{
    QMap<QString, QString> names;

    auto i = _formats.constBegin();
    while (i != _formats.constEnd()) {
        names.insert(i.key(), i.value()->name());
        ++i;
    }

    return names;
}

void QueryDataExporter::setFormatId(const QString & format)
{
    if (_formats.contains(format)) {
        _formatId = format;
    } else {
        Q_ASSERT(false);
    }
}

int QueryDataExporter::allRowsCount() const
{
    if (_model) {
        return _model->rowCount();
    } else {
        return 0;
    }
}

int QueryDataExporter::selectedRowsCount() const
{
    if (_selection) {
        return _selection->selectedRows().count();
    } else {
        return 0;
    }
}

} // namespace exporting
} // namespace utils
} // namespace meow
