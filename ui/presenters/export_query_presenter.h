#ifndef MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
#define MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H

#include <memory>
#include <QStringList>
#include <QItemSelectionModel>
#include "utils/exporting/query_data_exporter.h"

namespace meow {
namespace ui {

namespace models {
class BaseDataTableModel;
}

namespace presenters {

class ExportQueryPresenter
{
public:
    ExportQueryPresenter();

    void setData(models::BaseDataTableModel * model,
                 QItemSelectionModel * selection);

    void setModeClipboard();
    bool isModeClipboard() const;

    void setModeFile();
    bool isModeFile() const;

    void setOnlySelectedRows(bool selectedOnly) const;
    bool isSelectedRows() const;
    bool isAllRows() const;

    QString fileEncoding() const;
    void setFileEncoding(const QString & encoding);
    QStringList supportedFileEncodings() const;

    QString formatId() const;
    void setFormatId(const QString & format);
    QMap<QString, QString> formatNames() const;

    QString selectedRowsStats() const;
    QString allRowsStats() const;


private:
    std::unique_ptr<utils::exporting::QueryDataExporter> _exporter;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
