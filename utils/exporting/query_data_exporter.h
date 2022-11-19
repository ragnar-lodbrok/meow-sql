#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H

#include <QStringList>
#include <QMap>
#include <QItemSelectionModel>
#include <QTableView>
#include "query_data_export_formats/format.h"

namespace meow {

namespace ui {
namespace models {
class BaseDataTableModel;
}
}

namespace utils {
namespace exporting {

class QueryDataExporter : public QObject
{
    Q_OBJECT
public:
    QueryDataExporter();

    enum class Mode {
        Clipboard,
        File
    };

    enum class RowSelection {
        Complete,
        Selection
    };

    void setData(ui::models::BaseDataTableModel * model,
                 QItemSelectionModel * selection,
                 QTableView * tableView = nullptr);

    void setMode(Mode mode) {
        if (_mode == mode) return;
        _mode = mode;
        emit modeChanged();
    }
    Mode mode() const {
        return _mode;
    }

    void setRowSelection(RowSelection rowSelection) {
        _rowSelection = rowSelection;
    }
    RowSelection rowSelection() {
        return _rowSelection;
    }

    QStringList supportedFileEncodings() const;
    QString fileEncoding() const {
        return _encoding;
    }
    QString defaultFileEncoding() const {
        return "UTF-8";
    }
    void setFileEncoding(const QString & encoding) {
        _encoding = encoding;
    }

    QString filename() const {
        return _filename;
    }
    void setFilename(const QString & filename) {
        if (_filename == filename) return;
        _filename = filename;
        emit filenameChanged();
    }

    QMap<QString, QString> formatNames() const;
    const QMap<QString, QueryDataExportFormatPtr> & formats() const {
        return _formats;
    }
    void setFormatId(const QString & format);
    QString formatId() const {
        return _formatId;
    }
    QueryDataExportFormatPtr format() const {
        return _formats.value(_formatId);
    }

    int allRowsCount() const;
    int selectedRowsCount() const;

    void run();

    Q_SIGNAL void modeChanged();
    Q_SIGNAL void formatChanged();
    Q_SIGNAL void filenameChanged();

private:

    ui::models::BaseDataTableModel * _model = nullptr;
    QItemSelectionModel * _selection = nullptr;
    QTableView * _tableView = nullptr;

    Mode _mode = Mode::Clipboard;
    RowSelection _rowSelection = RowSelection::Complete;
    QString _filename;
    QString _encoding;
    QString _formatId = "csv";

    QMap<QString, QueryDataExportFormatPtr> _formats;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
