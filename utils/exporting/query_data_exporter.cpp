#include "query_data_exporter.h"
#include "query_data_export_formats/format_factory.h"
#include "ui/models/data_table_model.h"

#include <QTextCodec>
#include <algorithm> // std::sort

namespace meow {
namespace utils {
namespace exporting {

class QueryDataRowsIterator
{
public:
    void setData(ui::models::BaseDataTableModel * model)
    {
        _model = model;
        reset();
    }
    void setSelectionOnly(QItemSelectionModel * selection)
    {
        _selection = selection;
        reset();
    }
    bool hasNextRow() const
    {
        return _currentRow < _totalRows;
    }
    int getNextRow()
    {
        Q_ASSERT(hasNextRow());
        if (_selection) {
            return _selectedRows[_currentRow++];
        } else {
            return _currentRow++;
        }
    }
    void reset()
    {
        _currentRow = 0;
        _totalRows = 0;
        _selectedRows.clear();

        if (!_model) return;

        if (_selection) { // selection only
            QModelIndexList selectedRows = _selection->selectedRows();
            _selectedRows.reserve(selectedRows.count());
            for (const QModelIndex & rowIndex : selectedRows) {
                _selectedRows.push_back(rowIndex.row());
            }
            // selection is not sorted
            std::sort(_selectedRows.begin(), _selectedRows.end());
            _totalRows = _selectedRows.size();
        } else { // all data
            _totalRows = _model->rowCount();
        }
    }
private:
    ui::models::BaseDataTableModel * _model = nullptr;
    QItemSelectionModel * _selection = nullptr;
    int _currentRow = 0;
    int _totalRows = 0;
    std::vector<int> _selectedRows;
};

QueryDataExporter::QueryDataExporter()
    : QObject()
    , _encoding(defaultFileEncoding())
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
        if (_formatId != format) {
            _formatId = format;
            emit formatChanged();
        }
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

void QueryDataExporter::run()
{
    Q_ASSERT(_model != nullptr);

    QueryDataExportFormatPtr format = _formats.value(_formatId);
    Q_ASSERT(format);
    if (!format) return;

    format->setData(_model);

    QString fileName = "~\test.txt"; // TODO

    // TODO: overwrite file confirm
    // TODO: cursor hourglass

    QString tableName;

    auto tableData = dynamic_cast<ui::models::DataTableModel *>(_model);
    if (tableData != nullptr) {
        tableName = tableData->entity()->name();
    } else {
        // TODO: use "SQL" or actual SQL query?
        tableName = QObject::tr("Unknown Table");
    }

    std::unique_ptr<QFile> file;
    std::unique_ptr<QTextStream> stream;
    QString clipboardString;

    if (_mode == Mode::File) {
        file.reset(new QFile(fileName));
        QFile data("output.txt");
        if (!data.open(QFile::WriteOnly | QFile::Truncate)) {
            // TODO: show error?
            return;
        }

        stream.reset(new QTextStream(file.get()));
        QByteArray codec = _encoding.toUtf8();
        stream->setCodec(codec.data());
    } else {
        stream.reset(new QTextStream(&clipboardString));
    }

    QString header = format->header();

    *stream.get() << header;

    QueryDataRowsIterator rowsIterator;
    rowsIterator.setData(_model);
    if (_rowSelection == RowSelection::Selection) {
        rowsIterator.setSelectionOnly(_selection);
    }

    while (rowsIterator.hasNextRow()) {
        int rowIndex = rowsIterator.getNextRow();
        *stream.get() << format->row(rowIndex);
    }

    if (_mode == Mode::Clipboard) {
        stream->flush();

        qDebug() << "export\n\n" << clipboardString;
    }

}

} // namespace exporting
} // namespace utils
} // namespace meow
