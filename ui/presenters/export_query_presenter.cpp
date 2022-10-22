#include "export_query_presenter.h"
#include "utils/exporting/query_data_exporter.h"

namespace meow {
namespace ui {
namespace presenters {

ExportQueryPresenter::ExportQueryPresenter()
    : _exporter(new utils::exporting::QueryDataExporter())
{

}

void ExportQueryPresenter::setData(
    models::BaseDataTableModel * model,
    QItemSelectionModel * selection)
{
    _exporter->setData(model, selection);
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
    return QString("(%1 rows)").arg(_exporter->selectedRowsCount());
}

QString ExportQueryPresenter::allRowsStats() const
{
    return QString("(%1 rows)").arg(_exporter->allRowsCount());
}


} // namespace presenter
} // namespace ui
} // namespace meow
