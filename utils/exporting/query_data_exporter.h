#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H

#include <QStringList>
#include <QMap>
#include <QItemSelectionModel>
#include "query_data_export_formats/format_interface.h"

namespace meow {

namespace ui {
namespace models {
class BaseDataTableModel;
}
}

namespace utils {
namespace exporting {

class QueryDataExporter
{
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
                 QItemSelectionModel * selection);

    void setMode(Mode mode) {
        _mode = mode;
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

    QMap<QString, QString> formatNames() const;
    const QMap<QString, QueryDataExportFormatPtr> & formats() const {
        return _formats;
    }
    void setFormatId(const QString & format);
    QString formatId() const {
        return _formatId;
    }

    int allRowsCount() const;
    int selectedRowsCount() const;

private:

    ui::models::BaseDataTableModel * _model = nullptr;
    QItemSelectionModel * _selection = nullptr;

    Mode _mode = Mode::Clipboard;
    RowSelection _rowSelection = RowSelection::Complete;
    QString _encoding;
    QString _formatId = "csv";

    QMap<QString, QueryDataExportFormatPtr> _formats;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
