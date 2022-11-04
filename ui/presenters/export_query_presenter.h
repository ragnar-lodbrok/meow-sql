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

class ExportQueryPresenter : public QObject
{
    Q_OBJECT
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

    QVector<QPair<QString, QString>> optionValues() const;

    QString optionFieldSeparator() const;
    QString optionEncloser() const;
    QString optionLineTerminator() const;
    QString optionNullValue() const;

    bool isOptionEditableFieldSeparator() const;
    bool isOptionEditableEncloser() const;
    bool isOptionEditableLineTerminator() const;
    bool isOptionEditableNullValue() const;

    void setOptionFieldSeparator(const QString & value);
    void setOptionEncloser(const QString & value);
    void setOptionLineTerminator(const QString & value);
    void setOptionNullValue(const QString & value);

    bool optionBoolIncludeColumnNames() const;
    bool optionBoolIncludeAutoIncrementColumn() const;
    bool optionBoolIncludeSQLQuery() const;
    bool optionBoolRemoveLineBreaksFromContents() const;

    bool isOptionEditableIncludeColumnNames() const;
    bool isOptionEditableIncludeAutoIncrementColumn() const;
    bool isOptionEditableIncludeSQLQuery() const;
    bool isOptionEditableRemoveLineBreaksFromContents() const;

    void setOptionIncludeColumnNames(bool value);
    void setOptionIncludeAutoIncrementColumn(bool value);
    void setOptionIncludeSQLQuery(bool value);
    void setOptionRemoveLineBreaksFromContents(bool value);

    void run();

    Q_SIGNAL void formatChanged();

private:

    QString escapeOptionValue(const QString & str) const;
    QString unescapeOptionValue(const QString & str) const;

    std::unique_ptr<utils::exporting::QueryDataExporter> _exporter;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
