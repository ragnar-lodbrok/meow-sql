#include "export_query_presenter.h"
#include "utils/exporting/query_data_exporter.h"
#include "db/exception.h"
#include "settings/query_data_export_storage.h"
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

namespace meow {

using OptionsValue = utils::exporting::QueryDataExportFormat::OptionsValue;
using OptionsBool = utils::exporting::QueryDataExportFormat::OptionsBool;

namespace ui {
namespace presenters {

QString nameFilter(const utils::exporting::QueryDataExportFormatPtr & format)
{
    return QString("%1 (*.%2)")
            .arg(format->name(),
                 format->fileExtension());
}

QString allFilesFilter()
{
    return QObject::tr("All files (*)");
}

ExportQueryPresenter::ExportQueryPresenter()
    : QObject()
    , _exporter(new utils::exporting::QueryDataExporter())
{

    connect(_exporter.get(),
            &utils::exporting::QueryDataExporter::formatChanged,
            this,
            &ExportQueryPresenter::formatChanged);

    connect(_exporter.get(),
            &utils::exporting::QueryDataExporter::filenameChanged,
            this,
            &ExportQueryPresenter::filenameChanged);

    connect(_exporter.get(),
            &utils::exporting::QueryDataExporter::modeChanged,
            this,
            &ExportQueryPresenter::modeChanged);

    settings::QueryDataExportStorage storage;
    storage.loadTo(_exporter.get());
}

void ExportQueryPresenter::setData(
    models::BaseDataTableModel * model,
    QItemSelectionModel * selection,
    QTableView * tableView)
{
    _exporter->setData(model, selection, tableView);
}

void ExportQueryPresenter::setModeClipboard()
{
    _exporter->setMode(utils::exporting::QueryDataExporter::Mode::Clipboard);
}

bool ExportQueryPresenter::isModeClipboard() const
{
    return _exporter->mode()
            == utils::exporting::QueryDataExporter::Mode::Clipboard;
}

void ExportQueryPresenter::setModeFile()
{
    _exporter->setMode(utils::exporting::QueryDataExporter::Mode::File);
}

bool ExportQueryPresenter::isModeFile() const
{
    return _exporter->mode() == utils::exporting::QueryDataExporter::Mode::File;
}

QString ExportQueryPresenter::filename() const
{
    return _exporter->filename();
}

void ExportQueryPresenter::setFilename(
        const QString & filename,
        const QString & filenameFilter)
{

    if (_exporter->filename() == filename && filenameFilter.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(filename);
    QString inputExt = fileInfo.suffix();

    // If filename has no .ext add it by corresponding filter

    bool isAllFilesFilter = (allFilesFilter() == filenameFilter);
    QString filterFormatExt;
    QString filterFormatId;

    const QMap<QString, utils::exporting::QueryDataExportFormatPtr> & formats
            = _exporter->formats();

    auto i = formats.constBegin();
    while (i != formats.constEnd()) {

        utils::exporting::QueryDataExportFormatPtr format = i.value();

        if (isAllFilesFilter) {
            if (format->fileExtension() == inputExt) {
                filterFormatId = format->id();
                break;
            }
        } else {
            QString filter = nameFilter(format);

            if (filter == filenameFilter) {
                filterFormatExt = format->fileExtension();
                filterFormatId = format->id();
                break;
            }
        }

        ++i;
    }

    QString newFilename = filename;

    if (inputExt != filterFormatExt) {
        if (!filterFormatExt.isEmpty() && !filename.isEmpty()) {
            int lenToExt = newFilename.size() - inputExt.size();
            newFilename = newFilename.left(lenToExt);
            if (!newFilename.endsWith('.')) {
                newFilename += '.';
            }
            newFilename += filterFormatExt;
        }
    }

    _exporter->setFilename(newFilename);

    // Change format if we changed filter
    if (!filterFormatId.isEmpty()) {
        _exporter->setFormatId(filterFormatId);
    }
}

QStringList ExportQueryPresenter::filenameFilters() const
{
    utils::exporting::QueryDataExportFormatPtr curFormat = _exporter->format();

    QStringList filters;

    filters << nameFilter(curFormat); // put current format first

    const QMap<QString, utils::exporting::QueryDataExportFormatPtr> & formats
            = _exporter->formats();

    auto i = formats.constBegin();
    while (i != formats.constEnd()) {

        utils::exporting::QueryDataExportFormatPtr formatIt = i.value();
        ++i;

        if (curFormat == formatIt) {
            continue;
        }

        QString filter = nameFilter(formatIt);
        filters << filter;

    }

    filters << allFilesFilter();

    return filters;
}

void ExportQueryPresenter::setOnlySelectedRows(bool selectedOnly) const
{
    if (selectedOnly) {
        _exporter->setRowSelection(
            utils::exporting::QueryDataExporter::RowSelection::Selection);
    } else {
        _exporter->setRowSelection(
            utils::exporting::QueryDataExporter::RowSelection::Complete);
    }
}

bool ExportQueryPresenter::isSelectedRows() const
{
    return _exporter->rowSelection()
            == utils::exporting::QueryDataExporter::RowSelection::Selection;
}

bool ExportQueryPresenter::isAllRows() const
{
    return _exporter->rowSelection()
            == utils::exporting::QueryDataExporter::RowSelection::Complete;
}

QString ExportQueryPresenter::fileEncoding() const
{
    return _exporter->fileEncoding();
}

QStringList ExportQueryPresenter::supportedFileEncodings() const
{
    QStringList encodings = _exporter->supportedFileEncodings();
    encodings.sort();
    return encodings;
}

void ExportQueryPresenter::setFileEncoding(const QString & encoding)
{
    _exporter->setFileEncoding(encoding);
}

QString ExportQueryPresenter::formatId() const
{
    return _exporter->formatId();
}

void ExportQueryPresenter::setFormatId(const QString & format)
{
    _exporter->setFormatId(format);
}

QMap<QString, QString> ExportQueryPresenter::formatNames() const
{
    return _exporter->formatNames();
}

QString ExportQueryPresenter::selectedRowsStats() const
{
    // TODO: calc byte size?
    return QObject::tr("(%1 rows)").arg(_exporter->selectedRowsCount());
}

QString ExportQueryPresenter::allRowsStats() const
{
    return QObject::tr("(%1 rows)").arg(_exporter->allRowsCount());
}

QVector<QPair<QString, QString>> ExportQueryPresenter::optionValues() const
{

    QVector<QPair<QString, QString>> values = {
        {"\\t",     tr("Tab") + " [\\t]"},
        {",",       tr("Comma") + " [,]"},
        {";",       tr("Semicolon") + " [;]"},
        {"sep1",    "-"},
        {"'",       tr("Single quote") + " [']"},
        {"\"",      tr("Double quote") + " [\"]"},
        {"sep2",    "-"},
        {"\\r\\n",  tr("Windows linebreaks") + " [\\r\\n]"},
        {"\\n",     tr("Unix linebreaks") + " [\\n]"},
        {"\\r",     tr("Old macOS linebreaks") + " [\\r]"},
        {"sep3",    "-"},
        {QString(), tr("Empty string")},
    };

    return values;
}

QString ExportQueryPresenter::optionFieldSeparator() const
{
    return escapeOptionValue(_exporter->format()->fieldSeparator());
}

QString ExportQueryPresenter::optionEncloser() const
{
    return escapeOptionValue(_exporter->format()->encloser());
}

QString ExportQueryPresenter::optionLineTerminator() const
{
    return escapeOptionValue(_exporter->format()->lineTerminator());
}

QString ExportQueryPresenter::optionNullValue() const
{
    return escapeOptionValue(_exporter->format()->nullValue());
}

bool ExportQueryPresenter::isOptionEditableFieldSeparator() const
{
    return _exporter->format()->editableOptionsValue().contains(
        OptionsValue::FieldSeparator
    );
}

bool ExportQueryPresenter::isOptionEditableEncloser() const
{
    return _exporter->format()->editableOptionsValue().contains(
        OptionsValue::Encloser
    );
}

bool ExportQueryPresenter::isOptionEditableLineTerminator() const
{
    return _exporter->format()->editableOptionsValue().contains(
        OptionsValue::LineTerminator
    );
}

bool ExportQueryPresenter::isOptionEditableNullValue() const
{
    return _exporter->format()->editableOptionsValue().contains(
        OptionsValue::NullValue
    );
}

void ExportQueryPresenter::setOptionFieldSeparator(const QString & value)
{
    _exporter->format()->setOptionValue(OptionsValue::FieldSeparator,
                                        unescapeOptionValue(value));
}

void ExportQueryPresenter::setOptionEncloser(const QString & value)
{
    _exporter->format()->setOptionValue(OptionsValue::Encloser,
                                        unescapeOptionValue(value));
}

void ExportQueryPresenter::setOptionLineTerminator(const QString & value)
{
    _exporter->format()->setOptionValue(OptionsValue::LineTerminator,
                                        unescapeOptionValue(value));
}

void ExportQueryPresenter::setOptionNullValue(const QString & value)
{
    _exporter->format()->setOptionValue(OptionsValue::NullValue,
                                        unescapeOptionValue(value));
}

bool ExportQueryPresenter::optionBoolIncludeColumnNames() const
{
    return _exporter->format()->optionBool(
                OptionsBool::IncludeColumnNames);
}

bool ExportQueryPresenter::optionBoolIncludeAutoIncrementColumn() const
{
    return _exporter->format()->optionBool(
                OptionsBool::IncludeAutoIncrementColumn);
}

bool ExportQueryPresenter::optionBoolIncludeSQLQuery() const
{
    return _exporter->format()->optionBool(
                OptionsBool::IncludeSQLQuery);
}

bool ExportQueryPresenter::optionBoolRemoveLineBreaksFromContents() const
{
    return _exporter->format()->optionBool(
                OptionsBool::RemoveLineBreaksFromContents);
}

bool ExportQueryPresenter::isOptionEditableIncludeColumnNames() const
{
    return _exporter->format()->editableOptionsBool().contains(
                OptionsBool::IncludeColumnNames);
}

bool ExportQueryPresenter::isOptionEditableIncludeAutoIncrementColumn() const
{
    return _exporter->format()->editableOptionsBool().contains(
                OptionsBool::IncludeAutoIncrementColumn);
}

bool ExportQueryPresenter::isOptionEditableIncludeSQLQuery() const
{
    return _exporter->format()->editableOptionsBool().contains(
                OptionsBool::IncludeSQLQuery);
}

bool ExportQueryPresenter::isOptionEditableRemoveLineBreaksFromContents() const
{
    return _exporter->format()->editableOptionsBool().contains(
                OptionsBool::RemoveLineBreaksFromContents);
}

void ExportQueryPresenter::setOptionIncludeColumnNames(bool value)
{
    _exporter->format()->setOptionBool(
                OptionsBool::IncludeColumnNames, value);
}

void ExportQueryPresenter::setOptionIncludeAutoIncrementColumn(bool value)
{
    _exporter->format()->setOptionBool(
                OptionsBool::IncludeAutoIncrementColumn, value);
}

void ExportQueryPresenter::setOptionIncludeSQLQuery(bool value)
{
    _exporter->format()->setOptionBool(
                OptionsBool::IncludeSQLQuery, value);
}

void ExportQueryPresenter::setOptionRemoveLineBreaksFromContents(bool value)
{
    _exporter->format()->setOptionBool(
                OptionsBool::RemoveLineBreaksFromContents, value);
}

QString ExportQueryPresenter::escapeOptionValue(const QString & str) const
{
    QString res = str;
    res.replace(QLatin1Char('\t'), QLatin1String("\\t"));
    res.replace(QLatin1Char('\r'), QLatin1String("\\r"));
    res.replace(QLatin1Char('\n'), QLatin1String("\\n"));
    // why anyone ever wanted \b backspace?
    res.replace(QLatin1Char('\b'), QLatin1String("\\b"));
    //if (res.isEmpty()) return QString("\\0"); // TODO: use?
    return res;
}

QString ExportQueryPresenter::unescapeOptionValue(const QString & str) const
{
    QString res = str;
    res.replace(QLatin1String("\\t"), QLatin1String("\t"));
    res.replace(QLatin1String("\\r"), QLatin1String("\r"));
    res.replace(QLatin1String("\\n"), QLatin1String("\n"));
    res.replace(QLatin1String("\\b"), QLatin1String("\b"));
    return res;
}

bool ExportQueryPresenter::canRun() const
{
    if (isModeFile()) {
        return !filename().isEmpty();
    }

    return true;
}

QString ExportQueryPresenter::run()
{
    if (isModeFile()) {
        QFile file(filename());
        if (file.exists()) {
            QMessageBox msgBox;
            msgBox.setText(
                tr("File exists. Overwrite file %1?").arg(filename()));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setIcon(QMessageBox::Question);
            int ret = msgBox.exec();
            if (ret != QMessageBox::Yes) {
                return QString();
            }
        }
    }

    {
        settings::QueryDataExportStorage storage;
        storage.saveFrom(_exporter.get());
    }

    try {
        _exporter->run();
    } catch(meow::db::Exception & ex) {
        return ex.message();
    }

    return QString();
}

} // namespace presenter
} // namespace ui
} // namespace meow
