#include "query_data_export_storage.h"
#include "utils/exporting/query_data_exporter.h"
#include <QSettings>
#include <QCoreApplication>

namespace meow {
namespace settings {

const char settingsFilename[] = "QueryDataExport";

using Exporter = utils::exporting::QueryDataExporter;
using Format = utils::exporting::QueryDataExportFormat;
using FormatPtr = utils::exporting::QueryDataExportFormatPtr;

QString modeToString(Exporter::Mode mode)
{
    switch (mode) {
    case Exporter::Mode::Clipboard:
        return "clipboard";
    case Exporter::Mode::File:
        return "file";
    default:
        Q_ASSERT(false);
        return QString();
    }
}

Exporter::Mode stringToMode(const QString & str)
{
    if (str == "file") {
        return Exporter::Mode::File;
    } else if (str == "clipboard") {
        return Exporter::Mode::Clipboard;
    } else {
        return Exporter::defaultMode;
    }
}

QString rowSelectionToString(Exporter::RowSelection selection)
{
    switch (selection) {
    case Exporter::RowSelection::Complete:
        return "complete";
    case Exporter::RowSelection::Selection:
        return "selection";
    default:
        Q_ASSERT(false);
        return QString();
    }
}

Exporter::RowSelection stringToRowSelection(const QString & str)
{
    if (str == "complete") {
        return Exporter::RowSelection::Complete;
    } else if (str == "selection") {
        return Exporter::RowSelection::Selection;
    } else {
        return Exporter::defaultRowSelection;
    }
}

QString optionValueName(Format::OptionsValue option)
{
    switch (option) {
    case Format::OptionsValue::Encloser:
        return "encloser";
    case Format::OptionsValue::FieldSeparator:
        return "field_separator";
    case Format::OptionsValue::LineTerminator:
        return "line_terminator";
    case Format::OptionsValue::NullValue:
        return "null_value";
    default:
        Q_ASSERT(false);
        return QString();
    }
}

QString optionBoolName(Format::OptionsBool option)
{
    switch (option) {
    case Format::OptionsBool::IncludeAutoIncrementColumn:
        return "include_auto_increment_column";
    case Format::OptionsBool::IncludeColumnNames:
        return "include_column_names";
    case Format::OptionsBool::IncludeSQLQuery:
        return "include_sql_query";
    case Format::OptionsBool::RemoveLineBreaksFromContents:
        return "remove_line_breaks_from_contents";
    default:
        Q_ASSERT(false);
        return QString();
    }
}

void QueryDataExportStorage::saveFrom(
        utils::exporting::QueryDataExporter * exporter)
{
    Q_ASSERT(exporter);

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.setValue("mode", modeToString(exporter->mode()));
    settings.setValue("row_selection",
                      rowSelectionToString(exporter->rowSelection()));
    settings.setValue("filename", exporter->filename());
    settings.setValue("file_encoding", exporter->fileEncoding());
    settings.setValue("format_id", exporter->formatId());

    FormatPtr format = exporter->format();

    settings.beginGroup("format_options");

    std::vector<Format::OptionsValue> optionsValue = Format::enumOptionsValue();
    for (Format::OptionsValue option : optionsValue) {
        QString name = optionValueName(option);
        if (!name.isEmpty()) {
            settings.setValue(name, format->optionValue(option));
        }
    }

    std::vector<Format::OptionsBool> optionsBool = Format::enumOptionsBool();
    for (Format::OptionsBool option : optionsBool) {
        QString name = optionBoolName(option);
        if (!name.isEmpty()) {
            settings.setValue(name, format->optionBool(option));
        }
    }

    settings.endGroup();


}

void QueryDataExportStorage::loadTo(
        utils::exporting::QueryDataExporter * exporter)
{
    Q_ASSERT(exporter);

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    QString mode = settings.value("mode").toString();
    if (!mode.isEmpty()) {
        exporter->setMode(stringToMode(mode));
    }

    QString rowSelection = settings.value("row_selection").toString();
    if (!rowSelection.isEmpty()) {
        exporter->setRowSelection(stringToRowSelection(rowSelection));
    }

    QString filename = settings.value("filename").toString();
    if (!filename.isEmpty()) {
        exporter->setFilename(filename);
    }

    QString fileEncoding = settings.value("file_encoding").toString();
    if (!fileEncoding.isEmpty()) {
        exporter->setFileEncoding(fileEncoding);
    }

    QString formatId = settings.value("format_id").toString();
    if (formatId.isEmpty()) {
        return; // ignore format options
    } else {
        exporter->setFormatId(formatId);
    }

    FormatPtr format = exporter->format();
    if (!format) return;

    settings.beginGroup("format_options");

    std::vector<Format::OptionsValue> optionsValue = Format::enumOptionsValue();
    for (Format::OptionsValue option : optionsValue) {

        QString name = optionValueName(option);
        if (name.isEmpty()) continue;
        if (!settings.contains(name)) continue;

        QString value = settings.value(name).toString();
        format->setOptionValue(option, value);
    }

    std::vector<Format::OptionsBool> optionsBool = Format::enumOptionsBool();
    for (Format::OptionsBool option : optionsBool) {

        QString name = optionBoolName(option);
        if (name.isEmpty()) continue;
        if (!settings.contains(name)) continue;

        bool value = settings.value(name).toBool();
        format->setOptionBool(option, value);
    }

    settings.endGroup();
}

} // namespace meow
} // namespace settings
